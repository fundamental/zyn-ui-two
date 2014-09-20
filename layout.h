#ifdef __cplusplus
extern "C" {
#endif
typedef void *layout_t;

layout_t layoutCreate(void);
void layoutBoundBox(layout_t layout, float aspect, float rel);
void layoutDestroy(layout_t);
void layoutFlow(layout_t, int x, int y, int w, int h);
void layoutGet(layout_t, int, float *);
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
