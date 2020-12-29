
//
// This is a GUI support code to the chapters 12-16 of the book
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include "Window.h"
#include "Graph.h"
#include "GUI.h"

//------------------------------------------------------------------------------

namespace Graph_lib {

nWindow::nWindow(int ww, int hh, const string& title)
    :Fl_Window(ww,hh,title.c_str()),w(ww),h(hh)
{
    init();
}

//------------------------------------------------------------------------------

nWindow::nWindow(Point xy, int ww, int hh, const string& title)
    :Fl_Window(xy.x,xy.y,ww,hh,title.c_str()),w(ww),h(hh)
{ 
    init();
}

//------------------------------------------------------------------------------

void nWindow::init()
{
    resizable(this);
    show();
}

//------------------------------------------------------------------------------

void nWindow::draw()
{
    Fl_Window::draw();
    for (unsigned int i=0; i<shapes.size(); ++i) shapes[i]->draw();
}

//------------------------------------------------------------------------------

void nWindow::attach(Widget& w)
{
    begin();         // FTLK: begin attaching new Fl_Wigets to this window
    w.attach(*this); // let the Widget create its Fl_Wigits
    end();           // FTLK: stop attaching new Fl_Wigets to this window
}

//------------------------------------------------------------------------------

void nWindow::detach(Widget& b)
{
    b.hide();
}

//------------------------------------------------------------------------------

void nWindow::detach(Shape& s)
    // guess that the last attached will be first released
{
    for (unsigned int i = shapes.size(); 0<i; --i)    
        if (shapes[i-1]==&s)
            shapes.erase(shapes.begin()+(i-1));
}

//------------------------------------------------------------------------------

void nWindow::put_on_top(Shape& p) {
    for (int i=0; i<shapes.size(); ++i) {
        if (&p==shapes[i]) {
            for (++i; i<shapes.size(); ++i)
                shapes[i-1] = shapes[i];
            shapes[shapes.size()-1] = &p;
            return;
        }
    }
}

//------------------------------------------------------------------------------

int gui_main()
{
    return Fl::run();
}

//------------------------------------------------------------------------------

}; // of namespace Graph_lib
