typedef struct lidar_node {
    /* three elements read from lidar */
    float theta;
    float dist;
    float quality;

    struct lidar_node* next;

} *LidarNode;