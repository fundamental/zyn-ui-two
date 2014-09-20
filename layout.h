#ifdef __cplusplus
extern "C" {
#endif
typedef void *layout_t;

#define LAYOUT_NONE   (0)
#define LAYOUT_LABELS (1)
#define LAYOUT_DIR    (3<<1)
#define LAYOUT_LEFT   (1<<1)

layout_t layoutCreate(int traits);
void layoutBoundBox(layout_t layout, float aspect, float rel);
void layoutDestroy(layout_t);
void layoutFlow(layout_t, float x, float y, float w, float h);
void layoutGet(layout_t, int, float *);
void layoutGetLabel(layout_t, int, float *);

float *pad(float scale, float *bb);
float *boarder(float scale, float *bb);
#ifdef __cplusplus
}
#endif


/*
 * Layout is a cost optimization
 * knob: 1
 * mini_knob: 1/2
 * pulldown:  3
 * button:    fixed by labelsize
 *
 * Height is widget height + 30% for label space
 *
 * Fixed aspect widgets are either fixed by their height or their width for all
 * feasible bounding boxes
 *
 * Try layout consuming full width and if that exceeds the height bounding box,
 * then layout consuming full height
 */
