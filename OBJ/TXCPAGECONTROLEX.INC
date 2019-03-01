{$DEFINE PEAKTOP:OBJ/TXCPAGECONTROLEX.INC}
  //======================= PEAKTOP:OBJ/TXCPAGECONTROLEX.INC =====================
  function CreateTxcPageControlEx(aOwner:TComponent; aParent:TWinControl; const aName :string; const aLeft,aTop,aWidth,aHeight:Integer; const aAlign :TAlign):TxcPageControlEx;
  begin
    if(aOwner.FindComponent(aName) <> nil)then
      Result := TxcPageControlEx(aOwner.FindComponent(aName))
     else 
      Result := TxcPageControlEx.Create(aOwner);
    with Result do
      begin
      Name           := aName;
      Parent         := aParent;
      Left           := aLeft;
      Top            := aTop;
      Height         := aHeight;
      Width          := aWidth;
      Align          := aAlign;
      StyleManager   := Amunhotep.MainForm.StyleManager;
      Images         := Amunhotep.MainForm.ImageListMain;
      Gradient       := True;
      BoldCurrentTab := True;
      FlatColor      := Amunhotep.MainForm.StyleManager.Colors.Background;
      HotTrackStyle  := htsTab;
//      HotTrackColorSource := htcsHotTrackColorProp;
      ImageMargin    := 4;
      ImagePosition  := ipLeft;
      ShowMenuButton := True;
      ShowShadow     := False;
      SoftCorners    := False;
      SortTabMenu    := False;
      TabHeight      := Amunhotep.MainForm.ImageListMain.Height + 4;
      TabOrientation := toTop;
      TabSequence    := tsStandard;
      TabStyle       := tsSingleSlant;
      TextAlignment  := taLeftJustify;
      Transparent    := True;
      UseColoredTabs := True;
      end;
  end;
  //============================================================================
  procedure TxcPageControlEx_ActivateDefaultPage(aPageControl :TxcPageControlEx);
  begin
    if(aPageControl <> nil)then
      with aPageControl do  
        if(PageCount > 0)then
          begin
          ActivePage        := Pages[0];
          ActivePageDefault := Pages[0];
          end;
  end;
