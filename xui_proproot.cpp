#include "xui_propview.h"
#include "xui_proproot.h"

/*
//constructor
*/
xui_create_explain(xui_proproot)( void )
: m_ctrl(NULL)
{}

/*
//destructor
*/
xui_delete_explain(xui_proproot)( void )
{
	if (m_ctrl)
		m_ctrl->del_proproot(this);

	for (u32 i = 0; i < m_propkind.size(); ++i)
		delete m_propkind[i];
}

/*
//method
*/
xui_method_explain(xui_proproot, get_propkind, const xui_propkind_vec&	)( void ) const
{
	return m_propkind;
}
xui_method_explain(xui_proproot, has_propkind, bool						)( xui_propkind* propkind ) const
{
	for (u32 i = 0; i < m_propkind.size(); ++i)
	{
		if (m_propkind[i]->get_func() == propkind->get_func() &&
			m_propkind[i]->get_name() == propkind->get_name())
			return true;
	}

	return false;
}
xui_method_explain(xui_proproot, add_propkind, void						)( xui_propkind* propkind )
{
	m_propkind.push_back(propkind);
}
xui_method_explain(xui_proproot, non_ctrl,		void					)( void )
{
	m_ctrl = NULL;
	for (u32 i = 0; i < m_propkind.size(); ++i)
		m_propkind[i]->non_ctrl();
}
xui_method_explain(xui_proproot, get_ctrl,		xui_propview*			)( void )
{
	return m_ctrl;
}
xui_method_explain(xui_proproot, set_ctrl,		void					)( xui_propview* propview )
{
	m_ctrl = propview;
}