#ifdef __cplusplus
extern "C" {
#endif
typedef void *grid_t;
#define LAYOUT_NONE   (0)

grid_t gridCreate(int traits, int rows, int cols);
void gridBoundBox(grid_t grid, int row, int col, float aspect);
void gridRowWeight(grid_t grid, int row, float weight);
void gridColWeight(grid_t grid, int col, float weight);
void gridDestroy(grid_t);
void gridFlow(grid_t, float x, float y, float w, float h);
void gridGet(grid_t, int item, float *);
#ifdef __cplusplus
}
#endif
