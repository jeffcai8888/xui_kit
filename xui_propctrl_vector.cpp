#include "xui_convas.h"
#include "xui_button.h"
#include "xui_kindctrl.h"
#include "xui_propview.h"
#include "xui_propctrl_vector.h"

//////////////////////////////////////////////////////////////////////////
//propctrl_vector
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(xui_propctrl_vector, xui_propctrl);
/*
//create
*/
xui_method_explain(xui_propctrl_vector, create,				xui_propctrl*	)( xui_propdata* propdata )
{
	return new xui_propctrl_vector(propdata);
}

/*
//constructor
*/
xui_create_explain(xui_propctrl_vector)( xui_propdata* propdata )
: xui_propctrl()
{
	xui_propdata_vector* datavector = dynamic_cast<xui_propdata_vector*>(propdata);
	m_subxedit = new xui_propedit_number(this, datavector->get_numbtype(), datavector->get_interval());
	m_subyedit = new xui_propedit_number(this, datavector->get_numbtype(), datavector->get_interval());
	m_namectrl = new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_namectrl,	set_parent		)(this);
	xui_method_ptrcall(m_namectrl,	set_textalign	)(TEXTALIGN_LC);
	m_widgetvec.push_back(m_namectrl);

	xui_drawer*  subxname = m_subxedit->get_namectrl();
	xui_control* subxedit = m_subxedit->get_editctrl();
	xui_drawer*  subyname = m_subyedit->get_namectrl();
	xui_control* subyedit = m_subyedit->get_editctrl();
	xui_method_ptrcall(subxname,	set_text		)(L"X");
	xui_method_ptrcall(subyname,	set_text		)(L"Y");
	xui_method_ptrcall(subxname,	set_parent		)(this);
	xui_method_ptrcall(subyname,	set_parent		)(this);
	xui_method_ptrcall(subxedit,	set_parent		)(this);
	xui_method_ptrcall(subyedit,	set_parent		)(this);
	m_widgetvec.push_back(subxname);
	m_widgetvec.push_back(subyname);
	m_widgetvec.push_back(subxedit);
	m_widgetvec.push_back(subyedit);
}

/*
//destructor
*/
xui_delete_explain(xui_propctrl_vector)( void )
{
	delete m_subxedit;
	delete m_subyedit;
}

/*
//propdata
*/
xui_method_explain(xui_propctrl_vector,			on_linkpropdata,	void			)( bool selfupdate )
{
	if (selfupdate == false)
	{
		m_subxedit->reset();
		m_subyedit->reset();
		m_namectrl->set_text(m_propdata->get_name());
	}

	bool same = true;
	xui_propdata_vector* datavector = dynamic_cast<xui_propdata_vector*>(m_propdata);
	xui_vector<f64> value = datavector->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_vector* data = dynamic_cast<xui_propdata_vector*>(m_propdatavec[i]);
		if (data->get_value() != value)
		{
			same = false;
			break;
		}
	}

	if (same)
	{
		m_subxedit->set_value(value.x);
		m_subyedit->set_value(value.y);
	}
}
xui_method_explain(xui_propctrl_vector,			on_editvalue,		void			)( xui_propedit* sender )
{
	if (sender == m_subxedit)
	{
		f64 x = m_subxedit->get_value();
		for (u32 i = 0; i < m_propdatavec.size(); ++i)
		{
			xui_propdata_vector* data = dynamic_cast<xui_propdata_vector*>(m_propdatavec[i]);
			xui_vector<f64> value = data->get_value();
			value.x = x;
			data->set_value(value);
		}
	}
	if (sender == m_subyedit)
	{
		f64 y = m_subyedit->get_value();
		for (u32 i = 0; i < m_propdatavec.size(); ++i)
		{
			xui_propdata_vector* data = dynamic_cast<xui_propdata_vector*>(m_propdatavec[i]);
			xui_vector<f64> value = data->get_value();
			value.y = y;
			data->set_value(value);
		}
	}
}

/*
//override
*/
xui_method_explain(xui_propctrl_vector,			on_perform,			void			)( xui_method_args& args )
{
	xui_propctrl::on_perform(args);
	xui_rect2d<s32> rt = get_renderrt();
	xui_vector<s32> pt;
	xui_vector<s32> sz;
	xui_drawer*  subxname = m_subxedit->get_namectrl();
	xui_control* subxedit = m_subxedit->get_editctrl();
	xui_drawer*  subyname = m_subyedit->get_namectrl();
	xui_control* subyedit = m_subyedit->get_editctrl();
	s32 namewidth = xui_max(subxname->get_renderw(), subyname->get_renderw());
	s32 editwidth = (rt.get_w() - rt.get_w()/2 - 12 - 2*namewidth) / 2;
	//subxname
	pt.x = rt.get_w()/2;
	pt.y = 0;
	sz.w = namewidth;
	sz.h = xui_propview::default_lineheight;
	subxname->on_perform_pt(pt);
	subxname->on_perform_sz(sz);
	//subxedit
	pt.x = pt.x + subxname->get_renderw() + 4;
	pt.y = xui_propview::default_lineheight/2 - subxedit->get_renderh()/2;
	sz.w = editwidth;
	sz.h = subxedit->get_renderh();
	subxedit->on_perform_pt(pt);
	subxedit->on_perform_sz(sz);
	//subyname
	pt.x = pt.x + subxedit->get_renderw() + 4;
	pt.y = 0;
	sz.w = namewidth;
	sz.h = xui_propview::default_lineheight;
	subyname->on_perform_pt(pt);
	subyname->on_perform_sz(sz);
	//subyedit
	pt.x = pt.x + subyname->get_renderw() + 4;
	pt.y = xui_propview::default_lineheight/2 - subyedit->get_renderh()/2;
	sz.w = rt.get_w() - pt.x;
	sz.h = subyedit->get_renderh();
	subyedit->on_perform_pt(pt);
	subyedit->on_perform_sz(sz);
	//namectrl
	s32 indent = get_indent();
	m_namectrl->on_perform_w(rt.get_w()/2);
	m_namectrl->set_textoffset(xui_vector<s32>(indent, 0));
}

//////////////////////////////////////////////////////////////////////////
//propctrl_vector_button
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(xui_propctrl_vector_button, xui_propctrl_vector);
/*
//create
*/
xui_method_explain(xui_propctrl_vector_button,	create,				xui_propctrl*	)( xui_propdata* propdata )
{
	return new xui_propctrl_vector_button(propdata);
}

/*
//constructor
*/
xui_create_explain(xui_propctrl_vector_button)( xui_propdata* propdata )
: xui_propctrl_vector(propdata)
{
	m_namectrl->set_visible(false);

	m_zeroctrl = new xui_button(xui_vector<s32>(24, 16));
	m_zeroctrl->xm_buttonclick += new xui_method_member<xui_method_args, xui_propctrl_vector_button>(this, &xui_propctrl_vector_button::on_zeroctrlclick);
	xui_method_ptrcall(m_zeroctrl,	set_parent		)(this);
	xui_method_ptrcall(m_zeroctrl,	set_corner		)(3);
	xui_method_ptrcall(m_zeroctrl,	set_drawcolor	)(true);
	xui_method_ptrcall(m_zeroctrl,	set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_zeroctrl,	set_textalign	)(TEXTALIGN_CC);
	m_widgetvec.push_back(m_zeroctrl);

	xui_drawer*  subxname = m_subxedit->get_namectrl();
	xui_drawer*  subyname = m_subyedit->get_namectrl();
	xui_method_ptrcall(subxname,	set_textalign	)(TEXTALIGN_CC);
	xui_method_ptrcall(subyname,	set_textalign	)(TEXTALIGN_CC);
}

/*
//propdata
*/
xui_method_explain(xui_propctrl_vector_button,	on_linkpropdata,	void			)( bool selfupdate )
{
	xui_propctrl_vector::on_linkpropdata(selfupdate);

	if (selfupdate == false)
		m_zeroctrl->set_text(m_propdata->get_name());
}

/*
//override
*/
xui_method_explain(xui_propctrl_vector_button,	on_perform,			void			)( xui_method_args& args )
{
	xui_propctrl::on_perform(args);
	s32 indent = get_indent();

	xui_rect2d<s32> rt = get_renderrt();
	xui_vector<s32> pt;
	xui_vector<s32> sz;
	pt.x = indent;
	pt.y = xui_propview::default_lineheight/2 - m_zeroctrl->get_renderh()/2;
	m_zeroctrl->on_perform_pt(pt);

	xui_drawer*  subxname = m_subxedit->get_namectrl();
	xui_control* subxedit = m_subxedit->get_editctrl();
	xui_drawer*  subyname = m_subyedit->get_namectrl();
	xui_control* subyedit = m_subyedit->get_editctrl();
	s32 editwidth = (rt.get_w() - indent - m_zeroctrl->get_renderw() - 2*16) / 2;
	//subxedit
	pt.x = pt.x + m_zeroctrl->get_renderw();
	pt.y = 0;
	sz.w = 16;
	sz.h = xui_propview::default_lineheight;
	subxname->on_perform_pt(pt);
	subxname->on_perform_sz(sz);
	pt.x = pt.x +   subxname->get_renderw();
	pt.y = xui_propview::default_lineheight/2 - subxedit->get_renderh()/2;
	sz.w = editwidth;
	sz.h = subxedit->get_renderh();
	subxedit->on_perform_pt(pt);
	subxedit->on_perform_sz(sz);
	//subyedit
	pt.x = pt.x + editwidth;
	pt.y = 0;
	sz.w = 16;
	sz.h = xui_propview::default_lineheight;
	subyname->on_perform_pt(pt);
	subyname->on_perform_sz(sz);
	pt.x = pt.x +   subyname->get_renderw();
	pt.y = xui_propview::default_lineheight/2 - subyedit->get_renderh()/2;
	sz.w = rt.get_w() - pt.x;
	sz.h = subyedit->get_renderh();
	subyedit->on_perform_pt(pt);
	subyedit->on_perform_sz(sz);
}

/*
//event
*/
xui_method_explain(xui_propctrl_vector_button,	on_zeroctrlclick,	void			)( xui_component* sender, xui_method_args& args )
{
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_vector* data = dynamic_cast<xui_propdata_vector*>(m_propdatavec[i]);
		data->set_defvalue();
	}

	on_linkpropdata();
}