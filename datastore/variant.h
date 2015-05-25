
struct Unitary {};
struct Normal  {};
struct FixNum  {};
struct Bool    {};
struct String  {};
struct UnitaryVector {};
struct MidiCCType {};


//Typed Union
class DataField
{
    //Store For Underlying Virtual Type
    union {
        Unitary u;
        Normal  n;
        FixNum  f;
        Bool    b;
        String  s;
        MidiCCType m;
        UnitaryVector uv;
    } vtype;

    //Ideally there is also something that relates to the units
    //dB, ms, Hz, Q, %

    bool logScale;

    //Store For Underlying Real Type
    union {
        Unitary u;
        Normal  n;
        FixNum  f;
        Bool    b;
        String  s;
        MidiCCType m;
        UnitaryVector uv;
    } rtype;

    public:

};


//Binding View To Datastore
class Controller
{
    public:
        //Absolute Measurement Setter
        template<class T>
        Controller &operator=(T &t);

        //relative movement operator
        template<class T>
        Controller &operator+=(T &t);
};

//State of shift+control+alt+super
struct KeyboardState{};

//Drag origin dx dy pdx pdy
//Click up/down right left middle
//Scroll dy
//Hover
struct MouseEvent
{
    KeyboardState ks;
};


//Bounding box/null
//Drag/click/scroll/hover bitmask
//click mask yes, no, maybe
//keyboard state yes, no, maybe
struct MouseFilter
{
};


//Key Down, Key Up
struct KeyEvent
{
};

//up down modifiers
struct KeyFilter
{
};


struct View
{};

struct EventMask
{
    MouseFilter mf;
    int handle(MouseEvent);
    View &parent;
};

//A widget remembers the layout possition
//A widget holds the views which holds draw/view regionds
//


//A widget is MVCE
//M - The underlying Datastore
//    This hides the varying types and the network talk
//V - The draw routines which are placed by the layout engine
//C - The controller damages the view and translates events into actions
//E - The event boxes are dropped when the layout engine places the view


/*
 * Consider the creation of a new pane
 * - The widgets are created and placed within the layout hierarchy
 * - A transation starts with the datastore
 * - The widgets are told to request data from the datastore
 * - The transaction finishes or times out (disconnected backend)
 * - Widgets get data from the datastore
 * - Layout places the widgets
 * - Layout request a render breadth first
 * - Hierarchy is defined in linear fashion of event and draw boxes
 * - Widget knows locations of draw and event boxes
 * Now On interaction
 * - On an event event box list is traversed to find handler
 *   window->subwindow->widget->subwidget
 * - For keyboard a pseudo-stack of event handlers can be placed by an event box
 * - This single handler can commit suicide
 * Editing Text
 * - The user moves their mouse over the interesting widget
 * - They click their mouse which triggers an event handler
 * - The event handler spawns a keyboard handler and sets it as the active
 *   handler
 * - This handler talks to the controller as events are received
 */

/*
 * Debug mode:
 * - Show draw boxes
 * - Show layout boxes
 * - Show event boxes
 * - Dump datastore
 */

