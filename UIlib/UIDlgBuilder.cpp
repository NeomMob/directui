
#include "StdAfx.h"
#include "UIDlgBuilder.h"


ControlUI* CDialogBuilder::Create(const TCHAR* xml, IDialogBuilderCallback* pCallback /*= NULL*/)
{
   m_pCallback = pCallback;
   if (!m_xml.Load(xml))  return NULL;
   // NOTE: The root element is actually discarded since the _Parse() methods is
   //       parsing children and attaching to the current node.
   CMarkupNode root = m_xml.GetRoot();
   return _Parse(&root);
}

ControlUI* CDialogBuilder::CreateFromResource(UINT nRes, IDialogBuilderCallback* pCallback /*= NULL*/)
{
   HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceInstance(), MAKEINTRESOURCE(nRes), _T("XML"));
   if (hResource == NULL)  return NULL;
   HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceInstance(), hResource);
   if (hGlobal == NULL)  {
      FreeResource(hResource);
      return NULL;
   }
   CStdString sXML;
   sXML.Assign(static_cast<LPCSTR>(::LockResource(hGlobal)), ::SizeofResource(CPaintManagerUI::GetResourceInstance(), hResource));
   sXML.Replace(_T("\\n"), _T("\n"));
   ::FreeResource(hResource);
   return Create(sXML, pCallback);
}

ControlUI* CDialogBuilder::_Parse(CMarkupNode* root, ControlUI* parent)
{
   CDialogLayoutUI* pStretched = NULL;
   IContainerUI* pContainer = NULL;
   ControlUI* pReturn = NULL;
   for (CMarkupNode node = root->GetChild() ; node.IsValid(); node = node.GetSibling())  {
      const TCHAR* pstrClass = node.GetName();
      SIZE_T cchLen = _tcslen(pstrClass);
      ControlUI* ctrl = NULL;
      switch( cchLen)  {
      case 4:
         if (_tcscmp(pstrClass, _T("List")) == 0)                   ctrl = new ListUI;
         break;
      case 6:
         if (_tcscmp(pstrClass, _T("Canvas")) == 0)                 ctrl = new CanvasUI;
         else if (_tcscmp(pstrClass, _T("Button")) == 0)            ctrl = new ButtonUI;
         else if (_tcscmp(pstrClass, _T("Option")) == 0)            ctrl = new OptionUI;
         break;
      case 7:
         if (_tcscmp(pstrClass, _T("Toolbar")) == 0)                ctrl = new ToolbarUI;
         else if (_tcscmp(pstrClass, _T("TabPage")) == 0)           ctrl = new TabPageUI;
         else if (_tcscmp(pstrClass, _T("ActiveX")) == 0)           ctrl = new ActiveXUI;
         break;
      case 8:
         if (_tcscmp(pstrClass, _T("DropDown")) == 0)               ctrl = new DropDownUI;
         break;
      case 9:
         if (_tcscmp(pstrClass, _T("FadedLine")) == 0)              ctrl = new FadedLineUI;      
         else if (_tcscmp(pstrClass, _T("TaskPanel")) == 0)         ctrl = new TaskPanelUI;
         else if (_tcscmp(pstrClass, _T("Statusbar")) == 0)         ctrl = new StatusbarUI;
         else if (_tcscmp(pstrClass, _T("TabFolder")) == 0)         ctrl = new TabFolderUI;
         else if (_tcscmp(pstrClass, _T("TextPanel")) == 0)         ctrl = new TextPanelUI;
         break;
      case 10:
         if (_tcscmp(pstrClass, _T("ListHeader")) == 0)             ctrl = new ListHeaderUI;
         else if (_tcscmp(pstrClass, _T("ListFooter")) == 0)        ctrl = new ListFooterUI;
         else if (_tcscmp(pstrClass, _T("TileLayout")) == 0)        ctrl = new CTileLayoutUI;
         else if (_tcscmp(pstrClass, _T("ToolButton")) == 0)        ctrl = new ToolButtonUI;
         else if (_tcscmp(pstrClass, _T("ImagePanel")) == 0)        ctrl = new ImagePanelUI;
         else if (_tcscmp(pstrClass, _T("LabelPanel")) == 0)        ctrl = new CLabelPanelUI;
         break;
      case 11:
         if (_tcscmp(pstrClass, _T("ToolGripper")) == 0)            ctrl = new ToolGripperUI;
         else if (_tcscmp(pstrClass, _T("WhiteCanvas")) == 0)       ctrl = new CWhiteCanvasUI;
         else if (_tcscmp(pstrClass, _T("TitleShadow")) == 0)       ctrl = new CTitleShadowUI;
         break;
      case 12:
         if (_tcscmp(pstrClass, _T("WindowCanvas")) == 0)           ctrl = new CWindowCanvasUI;
         else if (_tcscmp(pstrClass, _T("DialogCanvas")) == 0)      ctrl = new CDialogCanvasUI;
         else if (_tcscmp(pstrClass, _T("DialogLayout")) == 0)      ctrl = new CDialogLayoutUI;
         else if (_tcscmp(pstrClass, _T("PaddingPanel")) == 0)      ctrl = new PaddingPanelUI;
         else if (_tcscmp(pstrClass, _T("WarningPanel")) == 0)      ctrl = new WarningPanelUI;
         break;
      case 13:
         if (_tcscmp(pstrClass, _T("SeparatorLine")) == 0)          ctrl = new CSeparatorLineUI;
         else if (_tcscmp(pstrClass, _T("ControlCanvas")) == 0)     ctrl = new CControlCanvasUI;
         else if (_tcscmp(pstrClass, _T("MultiLineEdit")) == 0)     ctrl = new CMultiLineEditUI;
         else if (_tcscmp(pstrClass, _T("ToolSeparator")) == 0)     ctrl = new ToolSeparatorUI;
         break;
      case 14:
         if (_tcscmp(pstrClass, _T("VerticalLayout")) == 0)         ctrl = new VerticalLayoutUI;
         else if (_tcscmp(pstrClass, _T("SingleLineEdit")) == 0)    ctrl = new CSingleLineEditUI;
         else if (_tcscmp(pstrClass, _T("SingleLinePick")) == 0)    ctrl = new CSingleLinePickUI;
         else if (_tcscmp(pstrClass, _T("NavigatorPanel")) == 0)    ctrl = new NavigatorPanelUI;
         else if (_tcscmp(pstrClass, _T("ListHeaderItem")) == 0)    ctrl = new CListHeaderItemUI;
         else if (_tcscmp(pstrClass, _T("GreyTextHeader")) == 0)    ctrl = new CGreyTextHeaderUI;
         break;
      case 15:
         if (_tcscmp(pstrClass, _T("ListTextElement")) == 0)        ctrl = new CListTextElementUI;
         else if (_tcscmp(pstrClass, _T("NavigatorButton")) == 0)   ctrl = new NavigatorButtonUI;      
         else if (_tcscmp(pstrClass, _T("TabFolderCanvas")) == 0)   ctrl = new CTabFolderCanvasUI;      
         break;
      case 16:
         if (_tcscmp(pstrClass, _T("ListHeaderShadow")) == 0)       ctrl = new CListHeaderShadowUI; 
         else if (_tcscmp(pstrClass, _T("HorizontalLayout")) == 0)  ctrl = new HorizontalLayoutUI;
         else if (_tcscmp(pstrClass, _T("ListLabelElement")) == 0)  ctrl = new CListLabelElementUI;
         else if (_tcscmp(pstrClass, _T("SearchTitlePanel")) == 0)  ctrl = new SearchTitlePanelUI;
         break;
      case 17:
         if (_tcscmp(pstrClass, _T("ToolbarTitlePanel")) == 0)   ctrl = new ToolbarTitlePanelUI;
         else if (_tcscmp(pstrClass, _T("ListExpandElement")) == 0)  ctrl = new CListExpandElementUI;
         break;
      }
      // User-supplied control factory
      if (ctrl == NULL && m_pCallback != NULL)  {
         ctrl = m_pCallback->CreateControl(pstrClass);
      }
      ASSERT(ctrl);
      if (ctrl == NULL)  return NULL;
      // Add children
      if (node.HasChildren())  {
         _Parse(&node, ctrl);
      }
      // Attach to parent
      if (parent != NULL)  {
         if (pContainer == NULL)  pContainer = static_cast<IContainerUI*>(parent->GetInterface(_T("Container")));
         ASSERT(pContainer);
         if (pContainer == NULL)  return NULL;
         pContainer->Add(ctrl);
      }
      // Process attributes
      if (node.HasAttributes())  {
         TCHAR szValue[500] = { 0 };
         SIZE_T cchLen = lengthof(szValue) - 1;
         // Set ordinary attributes
         int nAttributes = node.GetAttributeCount();
         for (int i = 0; i < nAttributes; i++)  {
            ctrl->SetAttribute(node.GetAttributeName(i), node.GetAttributeValue(i));
         }
         // Very custom attributes
         if (node.GetAttributeValue(_T("stretch"), szValue, cchLen))  {
            if (pStretched == NULL)  pStretched = static_cast<CDialogLayoutUI*>(parent->GetInterface(_T("DialogLayout")));
            ASSERT(pStretched);
            if (pStretched == NULL)  return NULL;
            UINT uMode = 0;
            if (_tcsstr(szValue, _T("move_x")) != NULL)  uMode |= UISTRETCH_MOVE_X;
            if (_tcsstr(szValue, _T("move_y")) != NULL)  uMode |= UISTRETCH_MOVE_Y;
            if (_tcsstr(szValue, _T("move_xy")) != NULL)  uMode |= UISTRETCH_MOVE_X | UISTRETCH_MOVE_Y;
            if (_tcsstr(szValue, _T("size_x")) != NULL)  uMode |= UISTRETCH_SIZE_X;
            if (_tcsstr(szValue, _T("size_y")) != NULL)  uMode |= UISTRETCH_SIZE_Y;
            if (_tcsstr(szValue, _T("size_xy")) != NULL)  uMode |= UISTRETCH_SIZE_X | UISTRETCH_SIZE_Y;
            if (_tcsstr(szValue, _T("group")) != NULL)  uMode |= UISTRETCH_NEWGROUP;
            if (_tcsstr(szValue, _T("line")) != NULL)  uMode |= UISTRETCH_NEWLINE;
            pStretched->SetStretchMode(ctrl, uMode);
         }
      }
      // Return first item
      if (pReturn == NULL)  pReturn = ctrl;
   }
   return pReturn;
}

