{$DEFINE PEAKTOP:OBJ/INTERFACE/$COMMON.INC}
{$IFNDEF PEAKTOP:OBJ/TACTION.INC}               {$I PEAKTOP:OBJ/TACTION.INC}               {$ENDIF}
{$IFNDEF PEAKTOP:OBJ/TXCGRADIENTPANELVISTA.INC} {$I PEAKTOP:OBJ/TXCGRADIENTPANELVISTA.INC} {$ENDIF}
{$IFNDEF PEAKTOP:OBJ/TXCTOOLBARBUTTON.INC}      {$I PEAKTOP:OBJ/TXCTOOLBARBUTTON.INC}      {$ENDIF}
const //================ PEAKTOP:OBJ/INTERFACE/$COMMON.INC =====================
  msg_interface_actload            = '���������';
  msg_interface_actload_hint       = '��������� �� �������� �����';
  msg_interface_actsave            = '���������';
  msg_interface_actsave_hint       = '��������� �� ������� ����';
  msg_interface_actcut             = '��������';
  msg_interface_actcut_hint        = '�������� � ����� ������';
  msg_interface_actcopy            = '����������';
  msg_interface_actcopy_hint       = '���������� � ����� ������';
  msg_interface_actpaste           = '��������';
  msg_interface_actpaste_hint      = '�������� �� ������ ������';
  msg_interface_actdelete          = '�������';
  msg_interface_actdelete_hint     = '������� ���������� ��������';
  msg_interface_actselectall       = '�������� ���';
  msg_interface_actselectall_hint  = '�������� ���';
  //============================================================================
  function Interface_CreateTAction(aOwner :TComponent; aParent:TWinControl; const aName, aCaption, aHint, aShortCut:string;
             const aGroupIndex, aImageIndex :Integer;  aOnExecute, aOnUpdate :TNotifyEvent):TCustomAction;
  var
    lkTBB :TxcToolBarButton;
  begin
    Result := CreateTAction(aOwner, aName, aCaption, aHint, aShortCut, aGroupIndex, aImageIndex); 
    Result.OnExecute := aOnExecute; 
    Result.OnUpdate  := aOnUpdate; 
    lkTBB := CreateTxcToolBarButton3(aParent, aParent, Result);
  end;
