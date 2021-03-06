#include "xui_global.h"
#include "xui_drawer.h"
#include "xui_button.h"
#include "xui_treeview.h"
#include "xui_treenode.h"
#include "xui_treedata.h"
#include "onity_restore.h"
#include "onity_mainform.h"

xui_implement_rtti(onity_restore, xui_window);

/*
//constructor
*/
xui_create_explain(onity_restore)( void )
: xui_window(xui_vector<s32>(480, 640), true)
{
	m_modal		= true;
	m_border	= xui_rect2d<s32>(10);
	m_sidestyle = SIDESTYLE_S;
	m_alignhorz = ALIGNHORZ_C;
	m_alignvert = ALIGNVERT_C;

	m_drawer	= new xui_drawer(xui_vector<s32>(128, 24));
	xui_method_ptrcall(m_drawer, ini_component		)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_drawer, set_text			)(L"Restore your unsaved file");

	std::vector<xui_treecolumn> columninfo;
	columninfo.push_back(xui_treecolumn(TREECOLUMN_BOOL, 25, L"flag", NULL, 0, true, TOGGLE_NORMAL));
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 200, L"name", NULL, 0, true));
	m_restore	= new xui_treeview(xui_vector<s32>(225), columninfo, 20, PLUSRENDER_NORMAL, false, false);
	xui_method_ptrcall(m_restore, ini_component		)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_restore, set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_restore, set_drawcolor		)(true);
	xui_method_ptrcall(m_restore, set_borderrt		)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_restore, set_hscrollauto	)(false);

	m_accept	= new xui_button(xui_vector<s32>(128, 24));
	xui_method_ptrcall(m_accept, xm_buttonclick		) += new xui_method_member<xui_method_args, onity_restore>(this, &onity_restore::on_buttonclick);
	xui_method_ptrcall(m_accept, ini_component		)(ALIGNHORZ_R, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_accept, set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_accept, set_corner			)(3);
	xui_method_ptrcall(m_accept, set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_accept, set_drawcolor		)(true);
	xui_method_ptrcall(m_accept, set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_accept, set_textalign		)(TEXTALIGN_CC);
	xui_method_ptrcall(m_accept, ini_drawer			)(L"Restore");

	m_bottom	= new xui_panel(xui_vector<s32>(128, 32));
	xui_method_ptrcall(m_bottom, ini_component		)(0, 0, DOCKSTYLE_B);
	xui_method_ptrcall(m_bottom, set_borderrt		)(xui_rect2d<s32>(0, 8, 0, 0));
	xui_method_ptrcall(m_bottom, add_child			)(m_accept);

	add_child(m_drawer);
	add_child(m_bottom);
	add_child(m_restore);
}

/*
//method
*/
xui_method_explain(onity_restore, load_unsavedfiles, void)(void)
{
	onity_mainform* mainform = onity_mainform::get_ptr();
	if (mainform)
	{
		for (int i = 0; i < mainform->get_unsavedfilesNum(); ++i)
		{
			m_restore->add_upmostnode(i, new xui_treedata(mainform->get_unsavedfileName(i), true));
		}
	}
}

/*
//callback
*/
xui_method_explain(onity_restore, on_accept,				void		)( xui_component* sender, xui_method_args&  args )
{
	restore();
	xui_window::on_accept(sender, args);
}

/*
//event
*/
xui_method_explain(onity_restore, on_buttonclick,		void		)( xui_component* sender, xui_method_args&  args )
{
	xui_method_args other_args;
	on_accept(this, other_args);
}

/*
//method
*/
xui_method_explain(onity_restore, restore,				void		)( void )
{
	for (int i = 0; i < m_restore->get_upmostnodecount(); ++i)
	{
		xui_treedata* data = m_restore->get_upmostnode(i)->get_linkdata();
		if (data)
		{
			std::wstring full = data->get_text(0);
			if (data->get_flag(0))
			{
				xui_global::cpy_file(xui_global::get_workpath() + full + L".tmp", xui_global::get_workpath() + full);
			}
			xui_global::del_file(xui_global::get_workpath() + full + L".tmp");
		}
	}
}


