#include "xui_window.h"
#include "xui_button.h"

inline void	test_button( xui_window* window )
{
	xui_button* button = NULL;
	button = xui_button::create(NULL);
	button->set_renderpt(xui_vector<s32>(10, 10));
	window->add_child(button);
	button = xui_button::create(NULL, L"Pivot");
	button->set_renderpt(xui_vector<s32>(50, 10));
	window->add_child(button);
}