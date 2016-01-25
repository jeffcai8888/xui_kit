#ifndef __xui_plusctrl_h__
#define __xui_plusctrl_h__

#include "xui_button.h"

enum
{
	PLUSRENDER_NORMAL,
	PLUSRENDER_SYMBOL,
};

class xui_plusctrl : public xui_button
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_plusctrl( u08 drawmode, xui_component* parent );

	/*
	//method
	*/
	bool						was_expanded	( void ) const;
	void						set_expanded	( bool flag );

	/*
	//method
	*/
	xui_method<xui_method_args>	xm_expand;

protected:
	/*
	//callback
	*/
	virtual void				on_mouseclick	( xui_method_mouse& args );
	virtual void				on_renderself	( xui_method_args&  args );

	/*
	//member
	*/
	u08							m_drawmode;
	bool						m_expanded;
};

#endif//__xui_plusctrl_h__