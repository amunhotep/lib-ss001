{$DEFINE PEAKTOP:IDE/ERP/PANELCLIENT.INC}
{$IFNDEF PEAKTOP:GLOBAL_VARIABLES.INC}                             {$I PEAKTOP:GLOBAL_VARIABLES.INC}                             {$ENDIF}
{$IFNDEF PEAKTOP:OBJ/TIMAGE.INC}                                   {$I PEAKTOP:OBJ/TIMAGE.INC}                                   {$ENDIF}
{$IFNDEF PEAKTOP:OBJ/TIMAGELIST.INC}                               {$I PEAKTOP:OBJ/TIMAGELIST.INC}                               {$ENDIF}
{$IFNDEF PEAKTOP:OBJ/TACTION.INC}                                  {$I PEAKTOP:OBJ/TACTION.INC}                                  {$ENDIF}
{$IFNDEF PEAKTOP:OBJ/TLABEL.INC}                                   {$I PEAKTOP:OBJ/TLABEL.INC}                                   {$ENDIF}
{$IFNDEF PEAKTOP:OBJ/TXCPANEL.INC}                                 {$I PEAKTOP:OBJ/TXCPANEL.INC}                                 {$ENDIF}
{$IFNDEF PEAKTOP:OBJ/TXCSTDPANEL.INC}                              {$I PEAKTOP:OBJ/TXCSTDPANEL.INC}                              {$ENDIF}
{$IFNDEF PEAKTOP:OBJ/TXCGRADIENTPANEL.INC}                         {$I PEAKTOP:OBJ/TXCGRADIENTPANEL.INC}                         {$ENDIF}
{$IFNDEF PEAKTOP:OBJ/TXCGRADIENTPANELVISTA.INC}                    {$I PEAKTOP:OBJ/TXCGRADIENTPANELVISTA.INC}                    {$ENDIF}
{$IFNDEF PEAKTOP:OBJ/TXCTOOLBARBUTTON.INC}                         {$I PEAKTOP:OBJ/TXCTOOLBARBUTTON.INC}                         {$ENDIF}
{$IFNDEF PEAKTOP:OBJ/TXCPAGECONTROLEX.INC}                         {$I PEAKTOP:OBJ/TXCPAGECONTROLEX.INC}                         {$ENDIF}
{$IFNDEF PEAKTOP:OBJ/TXCTABSHEET.INC}                              {$I PEAKTOP:OBJ/TXCTABSHEET.INC}                              {$ENDIF}
{$IFNDEF PEAKTOP:SYSUTILS/FILES/LOADIMAGELISTBITMAP.INC}           {$I PEAKTOP:SYSUTILS/FILES/LOADIMAGELISTBITMAP.INC}           {$ENDIF}
{$IFNDEF PEAKTOP:IDE/FIREBIRD/DIALOGS/$SETTINGS.INC}               {$I PEAKTOP:IDE/FIREBIRD/DIALOGS/$SETTINGS.INC}               {$ENDIF}
{$IFNDEF PEAKTOP:IDE/FIREBIRD/DIALOGS/DATABASE_SELECT_ACTIONS.INC} {$I PEAKTOP:IDE/FIREBIRD/DIALOGS/DATABASE_SELECT_ACTIONS.INC} {$ENDIF}
{$IFNDEF PEAKTOP:APPL/MANAGE/START/AUTO/DEFAULT.INC}               {$I PEAKTOP:APPL/MANAGE/START/AUTO/DEFAULT.INC}               {$ENDIF}
{$IFNDEF PEAKTOP:APPL/MANAGE/START/PARAMS/DEFAULT.INC}             {$I PEAKTOP:APPL/MANAGE/START/PARAMS/DEFAULT.INC}             {$ENDIF}
{$IFNDEF PEAKTOP:IDE/ERP/DATABASE/CONNECT.INC}                     {$I ./DATABASE/CONNECT.INC}                                   {$ENDIF}
{$IFNDEF PEAKTOP:IDE/ERP/MAINMENU_OBJNAMES.INC}                    {$I ./MAINMENU_OBJNAMES.INC}                                  {$ENDIF}
{$IFNDEF PEAKTOP:IDE/ERP/PANELCLIENT_OBJNAMES.INC}                 {$I ./PANELCLIENT_OBJNAMES.INC}                               {$ENDIF}  
{$IFDEF MSWINDOWS}
  {$IFNDEF PEAKTOP:COM/MICROSOFT/WINDOWS/SHELL32.DLL.INC}          {$I PEAKTOP:COM/MICROSOFT/WINDOWS/SHELL32.DLL.INC}            {$ENDIF}
  {$IFNDEF PEAKTOP:COM/MICROSOFT/WINDOWS/USER32.DLL.INC}           {$I PEAKTOP:COM/MICROSOFT/WINDOWS/USER32.DLL.INC}             {$ENDIF}
{$ENDIF}
  //===================== PEAKTOP:IDE/ERP/PANELCLIENT/DEFAULT.INC ==============
  procedure tsBaseConnect_CategoryButtons_OnButtonClicked(Sender: TObject; const Button: TButtonItem);
  var
    i, j         :Integer;
    lkFullNameIP :string;
    lkCodePage   :string;
    lkUserName   :string;
    lkUserRole   :string;
    lkUserPwd    :string;
    lkCmp        :TComponent;
  begin
    lkCmp := TComponent(Sender).Owner.FindComponent('FBFileConfig');
    with TxsDataBase(Button.Data) do
      begin
      lkFullNameIP := FullNameIP;
      lkCodePage   := CodePage;
      lkUserName   := UserName;
      lkUserRole   := UserRole;
      lkUserPwd    := UserPassword;
      if(lkCmp <> nil)then
        if ObjectInheritsFrom(lkCmp, 'TxFBFileConfig') then
          with TxFBFileConfig(lkCmp)do
            for i:=0 to Servers.Count-1 do
              for j:=0 to Servers[i].DataBases.Count-1 do
                if(Servers[i].DataBases[j] = TxsDataBase(Button.Data) )then
                  begin
                  lkUserName := Servers[i].UserName;
                  lkUserRole := Servers[i].UserRole;
                  lkUserPwd  := Servers[i].UserPassword;
                  break;
                  end;
      end;
    ConnectToDataBaseInternal(lkFullNameIP, lkCodePage, lkUserName, lkUserPwd, lkUserRole);
  end;
  //============================================================================
  procedure tsBaseAppl_CategoryButtons_OnButtonClicked(Sender: TObject; const Button: TButtonItem);
  var
    lkFilePath :string;
    lkFileDir  :string; 
    lkFileParam:string;
  begin
    lkFileDir  := ExtractFilePath(Amunhotep.ExeName);
    lkFilePath := Amunhotep.ExeName;
    lkFileParam:= Button.CaptionSmall;
    {$IFDEF MSWINDOWS}
    ShellExecute(Amunhotep.Handle, 'open', lkFilePath, lkFileParam, lkFileDir, 5 ); //SW_SHOW
    {$ENDIF}
  end;
  //============================================================================
  procedure PanelClient_LoadImages(aImageList :TImageList; const aInitDir:string; const aFileNames:array of string);
  var
    i :Integer;
  begin
    for i:=0 to Length(aFileNames)-1 do
      Files_LoadBitmapToImageList(aImageList, PrepareAbsoleteFileName(aInitDir+UpperCase(aFileNames[i])+FormatFloat('0000',aImageList.Height)+'X'+FormatFloat('0000',aImageList.Width)+'.BMP'));
  end;
  //============================================================================
  procedure PC_OnChange(Sender :TObject);
  var
    i :Integer;
  begin
    with TxcPageControlEx(Sender) do
      if(ActivePage <> nil)then
        begin
        TxcPanel(Owner).Colors.Foreground := ActivePage.Color;
        TxcPanel(Owner).Invalidate; 
        with TxcGradientPanel(Owner.FindComponent(obj_name_erp_amunhotepmainform_panelleft)) do
          begin
          Colors.Foreground := ActivePage.Color;
          Invalidate;
          for i:=0 to ControlCount-1 do
            Controls[i].InValidate;
          end;
        end;
  end;
  //============================================================================
  procedure actClose_OnExecute(Sender :TObject);
  var
    lkCmp :TComponent;
  begin
    lkCmp := TComponent(Sender).Owner.FindComponent(obj_name_erp_amunhotepmainform_panelclient);
    if(lkCmp <> nil)then try lkCmp.Free;except end;
  end;
  //============================================================================
  procedure MainPageControl_OnPageChange(Sender :TObject);
  begin
    with TxcPageControlEx(Sender).ActivePage do
      begin
      if(UpperCase(Name) = 'TSBASEURL')then
        begin
        if(not(ControlCount > 0))then
          with TWebBrowser.Create(TxcPageControlEx(Sender).ActivePage) do
            begin
            Parent := TxcPageControlEx(Sender).ActivePage;
            Align  := alClient;
            end;
        TWebBrowser(Controls[0]).Navigate('http://lom-bard.com.ua/');
        end;
      end;
  end;
  //============================================================================
  procedure Amunhotep_MainForm_PanelClient_OnDestroy(Sender :TObject);
  begin
    AutoFree;
  end;
  //============================================================================
  function Amunhotep_MainForm_PanelClient_Create:TxcPanel;
  var
    lkFileName    :string;
    lkTopBarBorder:TxcPanel;
    lkTopBar      :TxcGradientPanelVista;
    lkPnlLeft     :TxcGradientPanel;
    lkPC          :TxcPageControlEx;
    lkPCParams    :TxcPageControlEx;
    lkTS          :TxcTabSheet;
    lkPnlRight    :TxcStdPanel;
    lkILDB64      :TImageList; 
    lkILDB48      :TImageList; 
    lkILDB32      :TImageList; 
    lkILDB16      :TImageList; 
    lkCBTNS       :TCategoryButtons;
    lkFBFileConfig:TxFBFileConfig;
    lkAct         :TCustomAction;
    lkTBB         :TxcToolBarButton;
    lkLastTop     :Integer;
  begin
    Result   := CreateTxcPanel(Amunhotep.MainForm, Amunhotep.MainForm, obj_name_erp_amunhotepmainform_panelclient, '', alClient, 0,0,200,600,xbsNone);
//    Result.OnDestroy := @Amunhotep_MainForm_PanelClient_OnDestroy;
    lkILDB64 := CreateTImageListSimple(Result,'ilDatabase64',64,64); PanelClient_LoadImages(lkILDB64,'PEAKTOP:IMG/FIREBIRD/',['DATABASE','DATABASE_INS','DATABASE_UPD','DATABASE_DEL','SERVER','SERVER_INS','SERVER_UPD','SERVER_DEL']);
    lkILDB48 := CreateTImageListSimple(Result,'ilDatabase48',48,48); PanelClient_LoadImages(lkILDB48,'PEAKTOP:IMG/FIREBIRD/',['DATABASE','DATABASE_INS','DATABASE_UPD','DATABASE_DEL','SERVER','SERVER_INS','SERVER_UPD','SERVER_DEL']);
    lkILDB32 := CreateTImageListSimple(Result,'ilDatabase32',32,32); PanelClient_LoadImages(lkILDB32,'PEAKTOP:IMG/FIREBIRD/',['DATABASE','DATABASE_INS','DATABASE_UPD','DATABASE_DEL','SERVER','SERVER_INS','SERVER_UPD','SERVER_DEL']);
    lkILDB16 := CreateTImageListSimple(Result,'ilDatabase16',16,16); PanelClient_LoadImages(lkILDB16,'PEAKTOP:IMG/FIREBIRD/',['DATABASE','DATABASE_INS','DATABASE_UPD','DATABASE_DEL','SERVER','SERVER_INS','SERVER_UPD','SERVER_DEL']);
    lkTopBarBorder := CreateTxcPanel(Result,Result,'TopBarBorder','',alTop,0,0,56,600,xbsRaized);
    lkTopBar := CreateTxcGradientPanelVista(Result, lkTopBarBorder,'TopBar','', alClient, 0,0,54,600, xbsNone);
    with lkTopBar do
      begin
      StyleManager          := nil;
      Colors.HotTrack       := Amunhotep.MainForm.StyleManager.Colors.Foreground or $101010;
      Colors.Background     := $FFFFFF;
      Colors.Border         := $FFFFFF;
      Colors.HotTrackBorder := Amunhotep.MainForm.StyleManager.Colors.Foreground or $202020;
      end;  
    with CreateTImage(lkTopBar, lkTopBar, 'ImgLogo', alClient, 1, 1, 48, 380) do
      begin
      Center      := false;
      Stretch     := false;
      Transparent :=True;
      lkFileName  := StrAbsolutePath('../conf/ent/topbar.bmp', Amunhotep.ExeName);
      if FileExists(lkFileName)then
        Picture.Bitmap.LoadFromFile(lkFileName)
       else 
        begin
        lkFileName  := PrepareAbsoleteFileName('PEAKTOP:IMG/SPLASH/TOPBAR.BMP');
        if FileExists(lkFileName)then
          Picture.Bitmap.LoadFromFile(lkFileName);
        end;  
      end;
    lkPC := CreateTxcPageControlEx(Result, Result, 'MainPageControl', 0,0,100,100,alClient);
    with lkPC do
      begin
      Font.Name  := 'Verdana';
      Font.Color := Amunhotep.MainForm.StyleManager.Colors.Border; 
      Font.Size  := 10; 
      Images         := CreateTImageListSimple(lkPC,'ilPC',32,32);
      ShowMenuButton := false;
      TabHeight      := Images.Height + 4;
      TabOrientation := toLeft;
      TabSequence    := tsReverse;
      TabStyle       := tsSquareCorners;
      TabWidth       := 180;
      TextAlignment  := taLeftJustify;
      Transparent    := true;
      Margin         := TabHeight;
      OnPageChange   := @MainPageControl_OnPageChange;
      end;
    PanelClient_LoadImages(lkPC.Images,'PEAKTOP:IMG/FIREBIRD/'  ,['DATABASE_NEW','DATABASE']);  
    PanelClient_LoadImages(lkPC.Images,'PEAKTOP:IMG/CONFIG/'    ,['OPTIONS','OPTIONS']);
    PanelClient_LoadImages(lkPC.Images,'PEAKTOP:IMG/CONFIG/ERP/',['DBCONFIGURATOR']);
    PanelClient_LoadImages(lkPC.Images,'PEAKTOP:IMG/CONFIG/'    ,['APP_CLOSE','APP_EXIT','LIBSTD','BROWSER']);  
//    PanelClient_LoadImages(lkPC.Images,'PEAKTOP:IMG/CONFIG/ERP/',['AMUNHOTEP_']);  
//    lkTS := CreateTxcTabSheet(lkPC,'tsBaseNew'    ,'Создать'     , 0,Amunhotep.MainForm.StyleManager.Colors.Highlight);
    lkTS := CreateTxcTabSheet(lkPC,'tsBaseConnect','Подключиться', 1,Amunhotep.MainForm.StyleManager.Colors.Foreground);
    lkPC.ActivePage        := lkTS;
    lkPC.ActivePageDefault := lkTS;
    lkPnlRight := CreateTxcStdPanel(Result,lkTS,'PnlClientRight','',alRight, 0,0,600,128,xbsNone);
    with lkPnlRight do
      begin
      Color      := Amunhotep.MainForm.StyleManager.Colors.Window;
      Font.Name  := 'Verdana';
      Font.Style := [fsBold];
      Font.Color := Amunhotep.MainForm.StyleManager.Colors.Border; 
      Font.Size  := 10; 
      end;
    with CreateTImage(Result,lkPnlRight, 'ImageBgrRight', alBottom,0,0, 96,160) do
      begin 
      lkFileName := PrepareAbsoleteFileName('PEAKTOP:IMG/SPLASH/BACKGROUND0002.BMP');
      if FileExists(lkFileName)then
        Picture.Bitmap.LoadFromFile(lkFileName);
      end;
    lkCBTNS := TCategoryButtons.Create(Result);
    with lkCBTNS do
      begin
      Name                        := 'CategoryButtons';
      Parent                      := lkTS;
      BorderStyle                 := bsNone;
      Align                       := alClient;
      BackgroundGradientColor     := lkPnlRight.Color;
      BackgroundGradientDirection := gdLeftToRight; 
      ButtonFlow                  := cbfVertical;
      ButtonOptions               := [boGradientFill,boShowCaptions,boBoldCaptions,boUsePlusMinus,boCaptionOnlyBorder];
      Color                       := lkTS.Color;
      Font.Name                   := 'Verdana';
      Font.Color                  := Amunhotep.MainForm.StyleManager.Colors.Border;
      Font.Style                  := [fsBold];
      Font.Size                   := 10;
      GradientDirection           := gdTopToBottom;
      HotButtonColor              := Amunhotep.MainForm.StyleManager.Colors.Highlight;
      RegularButtonColor          := clNone;
      SelectedButtonColor         := Amunhotep.MainForm.StyleManager.Colors.Highlight;
      ShowHint                    := true;
      Images                      := lkILDB32;
      ButtonHeight                := Images.Height + 4;
      ButtonWidth                 := Images.Height * 5 + 4;
      OnButtonClicked             := @tsBaseConnect_CategoryButtons_OnButtonClicked;
      end;
    try
      lkFBFileConfig := LoadEncryptTxFBFileConfig(StrAbsolutePath('../conf/fb.conf', Amunhotep.ExeName));
    except
    end;  
    if(lkFBFileConfig = nil)then
      lkFBFileConfig := TxFBFileConfig.Create(nil);
    lkFBFileConfig.Name := 'FBFileConfig';
    Result.InsertComponent(lkFBFileConfig);
    FirebirdDialog_DatabaseSelect_FillCategories(lkFBFileConfig, lkCBTNS);
    FirebirdDialog_DataBaseSelect_CreateToolButtons(Result, lkPnlRight, lkCBTNS.Images, [5, 6, 7, 1, 2, 3]);
{
    lkTS := CreateTxcTabSheet(lkPC,'tsBaseAppl'  ,'Приложения', 2,Amunhotep.MainForm.StyleManager.Colors.Highlight);
    lkCBTNS := TCategoryButtons.Create(Result);
    with lkCBTNS do
      begin
      Name                        := 'CategoryButtonsAppl';
      BorderStyle                 := bsNone;
      Parent                      := lkTS;
      Align                       := alClient;
      BackgroundGradientColor     := Amunhotep.MainForm.StyleManager.Colors.Window;
      BackgroundGradientDirection := gdLeftToRight; 
      ButtonFlow                  := cbfVertical;
      ButtonOptions               := [boGradientFill,boShowCaptions,boBoldCaptions,boUsePlusMinus,boCaptionOnlyBorder];
      Color                       := lkTS.Color;
      Font.Name                   := 'Verdana';
      Font.Color                  := Amunhotep.MainForm.StyleManager.Colors.Border;
      Font.Style                  := [fsBold];
      Font.Size                   := 10;
      GradientDirection           := gdTopToBottom;
      HotButtonColor              := Amunhotep.MainForm.StyleManager.Colors.Highlight;
      RegularButtonColor          := clNone;
      SelectedButtonColor         := Amunhotep.MainForm.StyleManager.Colors.Highlight;
      ShowHint                    := false;
      Images                      := CreateTImageListSimple(lkCBTNS,'ilAppl',48,48);
      ButtonHeight                := Images.Height + 4;
      ButtonWidth                 := Images.Height * 8 + 4;
      OnButtonClicked             := @tsBaseAppl_CategoryButtons_OnButtonClicked;
      with Categories.Add do
        begin 
        ImageIndex    := -1;
        Caption       := lkTS.Caption+' '+UpperCase(Amunhotep.Name);
        Color         := Amunhotep.MainForm.StyleManager.Colors.HotTrackBorder;
        GradientColor := Amunhotep.MainForm.StyleManager.Colors.Border;
        TextColor     := Amunhotep.MainForm.StyleManager.Colors.Window;
        end;
      end;
    PanelClient_LoadImages(lkCBTNS.Images,'PEAKTOP:IMG/CONFIG/',['APP_APPL']);  
    with TClientDataSet.Create(nil) do
      begin
      FileName := AmunhotepFileNameStartParamsGet;
      if not FileExists(FileName)then
        Amunhotep_StartParams_Create(FileName);
      if not Active then  
        Open;
      First;
      while not EOF do
        begin
        with lkCBTNS.Categories[0].Items.Add do
          begin
          Caption     := FieldByName('NAME').AsString;
          CaptionSmall:= FieldByName('ID').AsString;
          Hint        := FieldByName('FILEPATH').AsString;
          ImageIndex  := 0;
          end; 
        Next;
        end;
      if Active then Close;
      Free;  
      end;
}
//    lkTS := CreateTxcTabSheet(lkPC,'tsBaseURL'   ,'lom-bard.com.ua'       , 8,Amunhotep.MainForm.StyleManager.Colors.Window);
    lkTS := CreateTxcTabSheet(lkPC,'tsBaseParams','Настройки'             , 4,Amunhotep.MainForm.StyleManager.Colors.Error);
    lkPCParams := CreateTxcPageControlEx(Result, lkTS, 'PageControlParams', 0,0,100,100,alClient);
    with lkPCParams do
      begin
      Font.Name  := 'Verdana';
      Font.Color := Amunhotep.MainForm.StyleManager.Colors.Border; 
      Font.Size  := 10; 
      Images         := lkPC.Images;
      TabHeight      := Images.Height;// + 4;
      TextAlignment  := taLeftJustify;
      Transparent    := true;
      end;
    lkTS := CreateTxcTabSheet(lkPCParams,'tsBaseStartAuto'  ,msg_manageinterface_startauto  , 3,$0080CFFF);
    ManageInterface_StartAuto_Create(lkTS, lkTS);
    lkPCParams.ActivePage        := lkTS;
    lkPCParams.ActivePageDefault := lkTS;
    lkTS := CreateTxcTabSheet(lkPCParams,'tsBaseStartParams',msg_manageinterface_startparams, 4,$00CF00FF);
    ManageInterface_StartParams_Create(lkTS, lkTS);

    lkLastTop := lkTopBarBorder.Top + lkTopBarBorder.Height + lkPC.Margin + (lkPC.PageCount * lkPC.TabHeight) + 8;   
    lkPnlLeft := CreateTxcGradientPanel(Result,Result,obj_name_erp_amunhotepmainform_panelleft,'',alNone,0,lkLastTop,Amunhotep.MainForm.ClientHeight - lkLastTop - 2,lkPC.TabWidth+4,xbsNone, gdRightToLeft);
    with lkPnlLeft do
      begin
      Font.Name   := 'Verdana';
      Font.Style  := [fsBold];
      Font.Color  := Amunhotep.MainForm.StyleManager.Colors.Border; 
      Font.Size   := 10;
      StyleManager:= nil; 
      end;
    lkLastTop := 0;
    lkAct := CreateTAction(Amunhotep.MainForm, 'actClose__'+Result.Name, 'Закрыть','Закрыть панель','Ctrl+C',-1,70);
    lkAct.OnExecute := @actClose_OnExecute;
    if(lkAct <> nil)then
      begin
      lkTBB := CreateTxcToolBarButton2(Result,lkPnlLeft, 'tbb'+lkAct.Name, 4,lkLastTop, lkPC.TabHeight, lkPnlLeft.Width, alTop, lkAct);
      with lkTBB do
        begin
        ShowCaption := true;
        Font.Name   := 'Verdana';
        Font.Style  := [fsBold];
        Font.Color  := Amunhotep.MainForm.StyleManager.Colors.Border; 
        Font.Size   := 10; 
        ImagesSmall := lkPC.Images;
        ImageIndex  := 5;
        lkLastTop   := lkLastTop + Height + 2;
        end; 
      end;
    lkAct := TCustomAction(Amunhotep.MainForm.FindComponent(obj_name_erpide_actbaseexit));
    if(lkAct <> nil)then
      begin
      lkTBB := CreateTxcToolBarButton2(Result,lkPnlLeft, 'tbb'+lkAct.Name, 4,lkLastTop, lkPC.TabHeight, lkPnlLeft.Width, alTop, lkAct);
      with lkTBB do
        begin
        ShowCaption := true;
        Font.Name   := 'Verdana';
        Font.Style  := [fsBold];
        Font.Color  := Amunhotep.MainForm.StyleManager.Colors.Border; 
        Font.Size   := 10; 
        ImagesSmall := lkPC.Images;
        ImageIndex  := 6;
        lkLastTop   := lkLastTop + Height + 2;
        end; 
      end;
    with CreateTLabel(Result,lkPnlLeft,'',0,lkLastTop,lkPC.TabHeight*2,200,alTop,taCenter) do
      begin
      lkLastTop   := lkLastTop + Height + 2;
      end;  
    with CreateTImage(Result,lkPnlLeft, 'ImageLogoLeft', alTop,0,lkLastTop,160,160)do
      begin 
      lkFileName := PrepareAbsoleteFileName(DefaultFirebirdDialogImageLogoError);
      if FileExists(lkFileName)then
        Picture.Bitmap.LoadFromFile(lkFileName);
      end;
    lkPC.OnChange := @PC_OnChange;
    PC_OnChange(lkPC);  
  end;  
