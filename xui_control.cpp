#include "xui_convas.h"
#include "xui_desktop.h"
#include "xui_linebox.h"
#include "xui_gridbox.h"
#include "xui_control.h"

xui_implement_rtti(xui_control, xui_component);

/*
//static
*/
const xui_colour xui_control::default_sidecolor = xui_colour(1.0f, 160.0f/255.0f);

/*
//constructor
*/
xui_create_explain(xui_control)( const xui_vector<s32>& size )
: xui_component(size)
{
	m_client	= m_render.get_sz();
	m_border	= xui_rect2d<s32>(0);
	m_scroll	= xui_vector<s32>(0);
	m_corner	= 0;
	m_drawcolor = false;
	m_sidestyle = 0;
	m_sidecolor = default_sidecolor;
	m_backpivot = xui_vector<f32>(0.0f);
	m_backscale = xui_vector<f32>(1.0f);
}

/*
//destructor
*/
xui_delete_explain(xui_control)( void )
{
	for (u32 i = 0; i < m_widgetvec.size(); ++i)
		delete m_widgetvec[i];
}

/*
//client
*/
xui_method_explain(xui_control, get_clientw,		s32						)( void ) const
{
	return m_client.w;
}
xui_method_explain(xui_control, get_clienth,		s32						)( void ) const
{
	return m_client.h;
}
xui_method_explain(xui_control, get_clientsz,		xui_vector<s32>			)( void ) const
{
	return m_client;
}
xui_method_explain(xui_control, set_clientsz,		void					)( const xui_vector<s32>& sz )
{
	if (m_client != sz)
	{
		m_client  = sz;

		xui_method_args      args; 
		on_setclientsz(      args); 
		xm_setclientsz(this, args);
	}
}

/*
//border
*/
xui_method_explain(xui_control, get_borderrt,		const xui_rect2d<s32>&	)( void ) const
{
	return m_border;
}
xui_method_explain(xui_control, set_borderrt,		void					)( const xui_rect2d<s32>& rt )
{
	if (m_border != rt)
	{
		m_client -= xui_vector<s32>(rt.ax+rt.bx-m_border.ax-m_border.bx, rt.ay+rt.by-m_border.ay-m_border.by);
		m_border  = rt;

		xui_method_args      args; 
		on_setborderrt(      args);
		xm_setborderrt(this, args);
	}
}

/*
//scroll
*/
xui_method_explain(xui_control, get_scrollx,		s32						)( void ) const
{
	return m_scroll.x;
}
xui_method_explain(xui_control, get_scrolly,		s32						)( void ) const
{
	return m_scroll.y;
}
xui_method_explain(xui_control, set_scrollx,		void					)( s32 x )
{
	m_scroll.x = x;
}
xui_method_explain(xui_control, set_scrolly,		void					)( s32 y )
{
	m_scroll.y = y;
}
xui_method_explain(xui_control, get_scrollpt,		const xui_vector<s32>&	)( void ) const
{
	return m_scroll;
}
xui_method_explain(xui_control, set_scrollpt,		void					)( const xui_vector<s32>& pt )
{
	m_scroll = pt;
}

/*
//corner
*/
xui_method_explain(xui_control, get_corner,			s32						)( void ) const
{
	return m_corner;
}
xui_method_explain(xui_control, set_corner,			void					)( s32 corner )
{
	m_corner = corner;
}

/*
//side
*/
xui_method_explain(xui_control, was_drawcolor,		bool					)( void ) const
{
	return m_drawcolor;
}
xui_method_explain(xui_control, set_drawcolor,		void					)( bool flag )
{
	m_drawcolor = flag;
}
xui_method_explain(xui_control, get_sidestyle,		u08						)( void ) const
{
	return m_sidestyle;
}
xui_method_explain(xui_control, set_sidestyle,		void					)( u08 sidestyle )
{
	m_sidestyle = sidestyle;

}
xui_method_explain(xui_control, get_sidecolor,		const xui_colour&		)( void ) const
{
	return m_sidecolor;
}
xui_method_explain(xui_control, set_sidecolor,		void					)( const xui_colour& color )
{
	m_sidecolor = color;
}

/*
//virtual
*/
xui_method_explain(xui_control,	get_screenpt,		xui_vector<s32>			)( void ) const
{
	xui_vector<s32> pt = m_render.get_pt() - m_scroll;
	return (m_parent == NULL) 
		? pt 
		: pt + m_parent->get_screenpt();
}
xui_method_explain(xui_control, get_cornerrt,		xui_rect2d<s32>			)( void ) const
{
	if (m_parent)
	{
		if (xui_issub_kindof(xui_linebox, m_parent) ||
			xui_issub_kindof(xui_gridbox, m_parent))
			return xui_dynamic_cast(xui_control, m_parent)->get_cornerrt((xui_component*)this);
	}

	return xui_rect2d<s32>(m_corner);
}
xui_method_explain(xui_control, get_cornerrt,		xui_rect2d<s32>			)( xui_component* widget ) const
{
	return xui_rect2d<s32>(0);
}

/*
//rectangle
*/
xui_method_explain(xui_control, get_clientrt,		xui_rect2d<s32>			)( void ) const
{
	return xui_rect2d<s32>(xui_vector<s32>(0, 0), m_client);
}
xui_method_explain(xui_control, get_renderrtins,	xui_rect2d<s32>			)( void ) const
{
	xui_rect2d<s32> rect = get_renderrt();
	rect.ax += m_border.ax;
	rect.ay += m_border.ay;
	rect.bx -= m_border.bx;
	rect.by -= m_border.by;
	return rect;
}

/*
//choose
*/
xui_method_explain(xui_control, choose,				xui_component*			)( const xui_vector<s32>& pt )
{
	if (m_popaction == NULL || m_popaction->was_play() == false)
	{
		if (m_enable && m_visible)
		{
			xui_component* component = choose_else(pt);
			if (component == NULL)
				component  = xui_component::choose(pt);

			return component;
		}
	}

	return NULL;
}
xui_method_explain(xui_control, choose_else,		xui_component*			)( const xui_vector<s32>& pt )
{
	xui_component* component = NULL;
	if (m_render.was_inside(pt))
	{
		xui_vector<s32> relative = pt - m_render.get_pt();
		for (s32 i = (s32)m_widgetvec.size()-1; i >= 0; --i)
		{
			if (component = m_widgetvec[i]->choose(relative))
				return component;
		}
	}

	return NULL;
}

/*
//update&render
*/
xui_method_explain(xui_control, update,				void					)( f32 delta )
{
	xui_component::update(delta);
	update_else(delta);
}
xui_method_explain(xui_control, update_else,		void					)( f32 delta )
{
	for (u32 i = 0; i < m_widgetvec.size(); ++i)
	{
		if (m_widgetvec[i]->was_enable() && m_widgetvec[i]->was_visible())
			m_widgetvec[i]->update(delta);
	}
}
xui_method_explain(xui_control, render,				void					)( void )
{
	xui_rect2d<s32> cornerrt = get_cornerrt   ();
	xui_rect2d<s32> renderrt = get_renderrtabs();
	xui_colour      color    = get_vertexcolor();

	f32 ox = -m_backpivot.x * renderrt.get_w() * m_backscale.x + m_backpivot.x * renderrt.get_w();
	f32 oy = -m_backpivot.y * renderrt.get_h() * m_backscale.y + m_backpivot.y * renderrt.get_h();
	f32 fw =  m_backscale.x * renderrt.get_w();
	f32 fh =  m_backscale.y * renderrt.get_h();
	renderrt.oft_x((s32)ox);
	renderrt.oft_y((s32)oy);
	renderrt.set_w((s32)fw);
	renderrt.set_h((s32)fh);
	if (renderrt.get_w() < cornerrt.ax ||
		renderrt.get_h() < cornerrt.ay)
		return;

	if (m_drawcolor)
	{
		xui_colour fill_color = get_rendercolor() * color;
		xui_convas::get_ins()->fill_round(renderrt, fill_color, cornerrt);
	}

	xui_component::render();
	if (m_popaction == NULL || m_popaction->was_play() == false)
	{
		render_else();
	}

	if (m_sidestyle)
	{
		renderrt.bx -= 1;
		renderrt.by -= 1;
		xui_colour side_color = m_sidecolor * color;
		xui_convas::get_ins()->draw_round(renderrt, side_color, cornerrt);

		if (m_sidestyle == SIDESTYLE_D)
		{
			xui_rect2d<s32> temp(
				renderrt.ax+1, 
				renderrt.ay+1, 
				renderrt.bx-1, 
				renderrt.by-1);

			side_color.r = 1.0f - side_color.r;
			side_color.g = 1.0f - side_color.g;
			side_color.b = 1.0f - side_color.b;
			xui_convas::get_ins()->draw_round(temp, side_color, cornerrt);
		}
	}
}
xui_method_explain(xui_control, render_else,		void					)( void )
{
	xui_rect2d<s32> cliprect = xui_convas::get_ins()->get_cliprect();
	xui_rect2d<s32> currrect = cliprect.get_inter(get_renderrtabs());
	xui_convas::get_ins()->set_cliprect(currrect);
	for (u32 i = 0; i < m_widgetvec.size(); ++i)
	{
		if (m_widgetvec[i]->was_visible() && currrect.get_inter(m_widgetvec[i]->get_renderrtabs()).was_valid())
			m_widgetvec[i]->render();
	}
	xui_convas::get_ins()->set_cliprect(cliprect);


	xui_method_args		args;
	xm_renderelse(this, args);
}

/*
//callback
*/
xui_method_explain(xui_control, on_setclientsz,		void					)( xui_method_args& args )
{
	perform();
}
xui_method_explain(xui_control, on_setborderrt,		void					)( xui_method_args& args )
{
	perform();
}
xui_method_explain(xui_control, on_renderback,		void					)( xui_method_args& args )
{

}
xui_method_explain(xui_control, on_renderself,		void					)( xui_method_args& args )
{

}
xui_method_explain(xui_control, on_perform,			void					)( xui_method_args& args )
{
	if (m_widgetvec.size() > 0)
	{
		xui_rect2d<s32> rt = get_renderrtins();
		perform_alignhorz(rt, m_widgetvec);
		perform_alignvert(rt, m_widgetvec);
		perform_dockstyle(rt, m_widgetvec);
	}
}