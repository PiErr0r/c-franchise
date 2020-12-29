
#include "../Graph.h"
#include "../Simple_window.h"

int main()
{
	Simple_window win (Point{100, 100}, 600, 400, "My window");
	win.wait_for_button();
}