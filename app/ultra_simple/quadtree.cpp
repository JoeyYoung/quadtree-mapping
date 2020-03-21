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

        /* if necessary, update corresponding node based on lidar information */
        void update_tree(LidarNode header){
            LidarNode p = header;
            while(p -> next != NULL){
                p = p -> next;

                /* transfer from dist-theta to  */
                float x = walker_x + p->dist * sin(p->theta * PI / 180);
                float y = walker_y + p->dist * cos(p->theta * PI / 180);

                TreeNode node = find_which_node(root, x, y);
                extend_children(node, x, y);
            }
        }

        /* update walker position based on odometry */
        void update_walker_pos(float x, float y, float theta){
            walker_x = x;
            walker_y = y;
            walker_theta = theta;

            printf("walker position updated ... \n");
        }
    
        /* begin from arbitrary node to extend */
        void extend_children(TreeNode node, float x, float y){
            /* can not extend */
            if(node -> radius < 0.5){
                return;
            }
            
            /* split four children */

            /* according to which children, call extend further */
        }
        
        /* begin from one node (usually root), find minimum node belongs to */
        TreeNode find_which_node(TreeNode node, float point_x, float point_y){
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

    printf("%f \n", tree.find_which_node(tree.root, 5.0, 5.0)->radius);

    /* prepare some one lidar_nodes array */
    LidarNode lidar_tail = (LidarNode)malloc(sizeof(struct lidar_node));
    LidarNode lidar_header = lidar_tail;
    LidarNode p;
    
    for(int i = 0; i < 360; i++){
        p = (LidarNode)malloc(sizeof(struct lidar_node));
        p->theta = (float)i;
        p->dist = 1000;
        p->quality = 60;

        lidar_tail->next = p;
        lidar_tail = p;
    }

    tree.update_tree(lidar_header);

    
    free(lidar_header);
    return 0;
}