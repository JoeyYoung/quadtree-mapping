#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.1415927

using namespace std;

typedef struct tree_node {
    /* the range of this node in the map */
    float t_range;
    float b_range;
    float r_range;
    float l_range;

    /* the radius of cuurrnt node */
    float radius;

    /* parent node */
    struct tree_node* parent;

    /* 
        four children inited with null
        right-top; left-top ...
     */
    struct tree_node* rt;
    struct tree_node* lt;
    struct tree_node* rb;
    struct tree_node* lb;
} *TreeNode;

typedef struct lidar_node {
    /* three elements read from lidar */
    float theta;
    float dist;
    float quality;

    struct lidar_node* next;

} *LidarNode;


class QuadTree{
    public:
        TreeNode root;

        /* current position of walker */
        float walker_x = 5.0;
        float walker_y = 5.0;
        float walker_theta = 0.0;

        /* build initial tree with only root node, and specific settings */
        QuadTree(TreeNode t):root(t){            
            /* area is 10 x 10 meters */
            root->radius = 10.0;
            
            root->t_range = 10.0;
            root->b_range = 0.0;
            root->r_range = 0.0;
            root->l_range = 10.0;

            printf("Quad Tree is inited with one root node ... \n");
        }

        ~QuadTree(){
            free(root);
        };

        /* if necessary, update corresponding node based on lidar info list */
        void update_tree(LidarNode header){
            LidarNode p = header;
            while(p -> next != NULL){
                p = p -> next;

                // TODO:
                /* transfer from dist-theta to x-y */
                float x = walker_x + p->dist * sin(p->theta * PI / 180);
                float y = walker_y + p->dist * cos(p->theta * PI / 180);

                TreeNode node = find_which_node(root, x, y);
                // printf("find node with radius: %f\n", node->radius);
                extend_children(node, x, y);
            }

            printf("update tree done \n");
        }

        // TODO:    
        /* update walker position based on odometry */
        void update_walker_pos(float x, float y, float theta){
            walker_x = x;
            walker_y = y;
            walker_theta = theta;

            printf("walker position updated ... \n");
        }
    
    private:
        /* begin from arbitrary node to extend */
        void extend_children(TreeNode node, float x, float y){
            // printf("extend children to node with radius: %f \n", node->radius);
            /* can not extend */
            if(node -> radius < 0.5){
                return;
            }
            
            /* split four children */
            TreeNode rt = (TreeNode)malloc(sizeof(struct tree_node));
            memset(rt, 0, sizeof(struct tree_node));
            rt->parent = node;
            rt->radius = node->radius / 2;
            rt->t_range = node->t_range;
            rt->b_range = (node->t_range + node->b_range) / 2;
            rt->l_range = (node->l_range + node->r_range) / 2;
            rt->r_range = node->r_range;

            TreeNode lt = (TreeNode)malloc(sizeof(struct tree_node));
            memset(lt, 0, sizeof(struct tree_node));
            lt->parent = node;
            lt->radius = node->radius / 2;
            lt->t_range = node->t_range;
            lt->b_range = (node->t_range + node->b_range) / 2;
            lt->l_range = node->l_range;
            lt->r_range = (node->l_range + node->r_range) / 2;           
            
            TreeNode lb = (TreeNode)malloc(sizeof(struct tree_node));
            memset(lb, 0, sizeof(struct tree_node));
            lb->parent = node;
            lb->radius = node->radius / 2;
            lb->t_range = (node->t_range + node->b_range) / 2;
            lb->b_range = node->b_range;
            lb->l_range = node->l_range;
            lb->r_range = (node->l_range + node->r_range) / 2;

            TreeNode rb = (TreeNode)malloc(sizeof(struct tree_node));
            memset(rb, 0, sizeof(struct tree_node));
            rb->parent = node;
            rb->radius = node->radius / 2;
            rb->t_range = (node->t_range + node->b_range) / 2;
            rb->b_range = node->b_range;
            rb->l_range = (node->l_range + node->r_range) / 2;
            rb->r_range = node->r_range;

            /* link children */
            node->lt = lt;
            node->rt = rt;
            node->rb = rb;
            node->lb = lb;

            /* according to which children, call extend further */
            TreeNode belong_node = find_which_node(node, x, y);
            extend_children(belong_node, x, y);
        }
        
        /* begin from one node (usually root), find leaf node belongs to */
        TreeNode find_which_node(TreeNode node, float point_x, float point_y){
            /* find until leaf node */
            if (node->lt == NULL){
                return node;
            }

            if(node->l_range < point_x <= (node->l_range + node->r_range) / 2 && (node->b_range + node->t_range) / 2 < point_y < node->t_range){
                return find_which_node(node->lt, point_x, point_y);
            }else if((node->l_range + node->r_range) < point_x <= node->r_range && (node->b_range + node->t_range) / 2 < point_y < node->t_range){
                return find_which_node(node->rt, point_x, point_y);
            }else if(node->l_range < point_x <= (node->l_range + node->r_range) / 2 && node->b_range < point_y <= (node->b_range + node->t_range) / 2){
                return find_which_node(node->lb, point_x, point_y);
            }else
                return find_which_node(node->rb, point_x, point_y);
        }
};

int main(int argc, const char * argv[]){
    /* the initial tree is one node with null children, need expended */
    TreeNode node = (TreeNode)malloc(sizeof(struct tree_node));

    /* init value to 0 and pointer to null */
    memset(node, 0, sizeof(struct tree_node));

    QuadTree tree = QuadTree(node);

    /* prepare some one lidar_nodes array */
    LidarNode lidar_tail = (LidarNode)malloc(sizeof(struct lidar_node));
    LidarNode lidar_header = lidar_tail;
    LidarNode p;
    
    for(int i = 0; i < 1; i++){
        p = (LidarNode)malloc(sizeof(struct lidar_node));
        memset(p, 0, sizeof(struct lidar_node));
        p->theta = (float)45;
        p->dist = 1;
        p->quality = 60;

        lidar_tail->next = p;
        lidar_tail = p;
    }

    tree.update_tree(lidar_header);
    
    free(lidar_header);
    return 0;
}