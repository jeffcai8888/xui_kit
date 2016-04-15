#include "xui_treeview.h"
#include "onity_mainform.h"
#include "onity_inspector.h"
#include "onity_filedata.h"
#include "onity_tileview.h"
#include "onity_fileview.h"

xui_implement_rtti(onity_fileview, xui_control);

/*
//constructor
*/
xui_create_explain(onity_fileview)( void )
: xui_control(xui_vector<s32>(0))
{
	std::vector<xui_treecolumn> columninfo;
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 200, L"name", NULL, 0, true));
	m_lineview = new xui_treeview(xui_vector<s32>(200), columninfo, 20, PLUSRENDER_NORMAL, false, false);
	xui_method_ptrcall(m_lineview,	xm_selectedchange	) += new xui_method_member<xui_method_args,  onity_fileview>(this, &onity_fileview::on_lineviewselection);
	xui_method_ptrcall(m_lineview,	xm_mousedoubleclick	) += new xui_method_member<xui_method_mouse, onity_fileview>(this, &onity_fileview::on_lineviewdoubleclk);
	xui_method_ptrcall(m_lineview,	set_parent			)(this);
	xui_method_ptrcall(m_lineview,	ini_component		)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_lineview,	ini_component		)(true, true);
	xui_method_ptrcall(m_lineview,	set_drawcolor		)(false);
	xui_method_ptrcall(m_lineview,	set_hscrollauto		)(false);

	m_tileview = new onity_tileview;
	xui_method_ptrcall(m_tileview, set_parent			)(this);
	xui_method_ptrcall(m_tileview, ini_component		)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_tileview, ini_component		)(true, false);

	m_widgetvec.push_back(m_tileview);
	m_widgetvec.push_back(m_lineview);
}

/*
//method
*/
xui_method_explain(onity_fileview, get_lineview, xui_treeview*)( void )
{
	return m_lineview;
}
xui_method_explain(onity_fileview, get_tileview, onity_tileview*)( void )
{
	return m_tileview;
}

/*
//event
*/
xui_method_explain(onity_fileview, on_lineviewselection,	void)( xui_component* sender, xui_method_args&  args )
{
	xm_lineviewselection(this, args);
}
xui_method_explain(onity_fileview, on_lineviewdoubleclk,	void)( xui_component* sender, xui_method_mouse& args )
{
	xm_lineviewdoubleclk(this, args);
}