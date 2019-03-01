{$DEFINE PEAKTOP:IDE/$COMMON/LIBRARYUPDATES.INC}
{$IFNDEF PEAKTOP:OBJ/TMEMO.INC}                         {$I PEAKTOP:OBJ/TMEMO.INC}                     {$ENDIF}
{$IFNDEF PEAKTOP:OBJ/TLABEL.INC}                        {$I PEAKTOP:OBJ/TLABEL.INC}                    {$ENDIF}
{$IFNDEF PEAKTOP:APPL/CONSOLE/CAPTURED_WINDOWS.INC}     {$I PEAKTOP:APPL/CONSOLE/CAPTURED_WINDOWS.INC} {$ENDIF}
{$IFNDEF PEAKTOP:IDE/$COMMON/FORMCOMMON.INC}            {$I PEAKTOP:IDE/$COMMON/FORMCOMMON.INC}        {$ENDIF}
{$IFNDEF PEAKTOP:IDE/$COMMON/LIBRARYUPDATES_NAMES.INC}  {$I ./LIBRARYUPDATES_NAMES.INC}                {$ENDIF}
{$IFNDEF PEAKTOP:IDE/$COMMON/LIBRARYUPDATES_LANG.INC}   {$I ./LIBRARYUPDATES_LANG.INC}                 {$ENDIF}
{$IFNDEF PEAKTOP:IDE/$COMMON/LIBRARYUPDATES_COMMIT.INC} {$I ./LIBRARYUPDATES_COMMIT.INC}               {$ENDIF}
  //======================= PEAKTOP:IDE/$COMMON/LIBRARYUPDATES.INC =============
  function TLibUpdateForm_RunCaptured(const aWorkingDir, aExeFileName, aExeFileParams:string):string;
  var
    i    :Integer;
    lkSL :TStrings;
  begin
    {$IFDEF MSWINDOWS}
    Result := RunCaptured(aWorkingDir, aExeFileName, aExeFileParams);  
    {$ENDIF}
    lkSL := TStringList.Create;
    lkSL.Text := Result;
    for i:=0 to lkSL.Count-1 do
      lkSL[i] := #$20#$20#$20#$20#$20#$20#$20#$20+lkSL[i];
    Result := lkSL.Text;
    lkSL.Free;  
  end;
  //============================================================================
  function TLibUpdateForm_GetSVN(var aExePath:string):Boolean;
  begin
    Result := false;
    aExePath := StrAbsolutePath('../../svn/bin/svn.exe', Amunhotep.ExeName);
    {$IFDEF MSWINDOWS}
    aExePath := StrReplaceStr(aExePath, '/', '\');
    {$ENDIF}
    if not FileExists(aExePath)then
      begin
      Dialogs.MessageDlg('Не найден SVN-клиент, необходимый для обновления программы.', mtError, [mbOk]);
      exit;
      end;
    Result := true;  
  end;
  //============================================================================
  function TLibUpdateForm_GetUpdateURL(var aURL, aUserName, aUserPwd:string):Boolean;
  var
    lkFN :string;
  begin
    aURL   := '';
    Result := false;
    lkFN := PrepareAbsoleteFileName('PEAKTOP:IDE/$COMMON/LIBRARYUPDATES.CONF');
    if not FileExists(lkFN)then
      begin
      Dialogs.MessageDlg('Не найден файл настроек подключения к репозитарию, необходимый для обновления программы.'+#13#10#13#10+lkFN, mtError, [mbOk]);
      exit;
      end;                             
    with TStringList.Create do
      try
        Text      := StringsLoadFromFile(lkFN);
        aURL      := Values['amunhotep_library_update_url'];
        aUserName := Values['amunhotep_library_update_username'];
        aUserPwd  := Values['amunhotep_library_update_userpwd'];
      finally  
        Free;
      end;
    Result := (StrTrimAll(aURL) <> '');    
  end;
  //============================================================================
  procedure TLibUpdateForm_actUpdateVer_OnExecute(Sender :TObject);
  var
    lkMemo     :TMemo;
    lkSVN      :string;
    lkPath     :string;
    lkSL       :TStrings;
    lkVer      :string;
  begin
    lkVer  := 'не известно';
    lkMemo := TMemo(TComponent(Sender).Owner.FindComponent(obj_name_common_memo));
    if not TLibUpdateForm_GetSVN(lkSVN) then exit;
    lkSVN := StrReplaceStr(lkSVN, 'svn.exe', 'svnversion.exe');
    lkPath := ' '+Amunhotep.SourcePath;
    lkSL := TStringList.Create;    
    lkSL.Text := TLibUpdateForm_RunCaptured(ExtractFilePath(lkSVN), lkSVN, lkPath);  
    lkMemo.Text := lkMemo.Text +FormatDateTime('dd.mm.yyyy hh:nn:ss',Now)+': '+mgs_library_updates_svn_check+'.'+#13#10+
      #$20#$20#$20#$20+lkSVN+lkPath+#13#10+lkSL.Text+#13#10#13#10;
    if(lkSL.Count > 0)then  
      lkVer := StrCiphersOnlyInt(Copy(lkSL[lkSL.Count-1], Pos(':', lkSL[lkSL.Count-1])+1, Length(lkSL[lkSL.Count-1]) - Pos(':', lkSL[lkSL.Count-1]) ));
    lkSL.Free;
    TCommonForm_SetCaption(TComponent(Sender).Owner, mgs_library_updates_caption + ' ['+mgs_library_updates_revision + ': ' + lkVer + ']');
  end;
  //============================================================================
  procedure TLibUpdateForm_actUpdateClr_OnExecute(Sender :TObject);
  var
    lkMemo     :TMemo;
    lkSVN      :string;
    lkPath     :string;
    lkUpdateURL:string;
    lkUserName :string;
    lkUserPwd  :string;
  begin
    lkMemo := TMemo(TComponent(Sender).Owner.FindComponent(obj_name_common_memo));
    if not TLibUpdateForm_GetSVN(lkSVN) then exit;
    if not TLibUpdateForm_GetUpdateURL(lkUpdateURL, lkUserName, lkUserPwd)then exit;
    ChDir(Amunhotep.SourcePath);
    if not DirectoryExists('.svn' ) then CreateDir('.svn');
    ChDir(Amunhotep.SourcePath+'.svn');
    if not DirectoryExists('tmp')then CreateDir('tmp');
    lkPath := ' cleanup '+Amunhotep.SourcePath+' --non-interactive --no-auth-cache --username '+lkUserName+' --password "'+lkUserPwd+'" ';
    lkMemo.Text := lkMemo.Text + FormatDateTime('dd.mm.yyyy hh:nn:ss',Now)+': '+mgs_library_updates_svn_cleanup+'.'+#13#10+#$20#$20#$20#$20+lkSVN+' '+lkPath+#13#10+
      TLibUpdateForm_RunCaptured(ExtractFilePath(lkSVN), lkSVN, lkPath)+#13#10#13#10;
  end;
  //============================================================================
  procedure TLibUpdateForm_actUpdateRun_OnExecute(Sender :TObject);
  var
    lkMemo     :TMemo;
    lkSVN      :string;
    lkPath     :string;
    lkUpdateURL:string;
    lkUserName :string;
    lkUserPwd  :string;
  begin
    lkMemo := TMemo(TComponent(Sender).Owner.FindComponent(obj_name_common_memo));
    if not TLibUpdateForm_GetSVN(lkSVN) then exit;
    if not TLibUpdateForm_GetUpdateURL(lkUpdateURL, lkUserName, lkUserPwd)then exit;
    ChDir(Amunhotep.SourcePath);
    if not DirectoryExists('.svn' ) then CreateDir('.svn');
    ChDir(Amunhotep.SourcePath+'.svn');
    if not DirectoryExists('tmp')then TLibUpdateForm_actUpdateClr_OnExecute(Sender);

    lkPath := ' co '+lkUpdateURL+' '+Amunhotep.SourcePath+' --non-interactive --no-auth-cache --username '+lkUserName+' --password "'+lkUserPwd+'" ';
    lkMemo.Text := lkMemo.Text +FormatDateTime('dd.mm.yyyy hh:nn:ss',Now)+': '+mgs_library_updates_svn_update+'.'+#13#10+#$20#$20#$20#$20+lkSVN+lkPath+#13#10+
      TLibUpdateForm_RunCaptured(ExtractFilePath(lkSVN), lkSVN, lkPath)+#13#10+#13#10;

    TLibUpdateForm_actUpdateVer_OnExecute(Sender);  
  end;
  //============================================================================
  procedure TLibUpdateForm_actCommitSVN_OnExecute(Sender :TObject);
  var
    lkMemo     :TMemo;
  begin
    lkMemo := TMemo(TComponent(Sender).Owner.FindComponent(obj_name_common_memo));
    lkMemo.Text := lkMemo.Text +AmunhotepStandardLibrary_CommitSVN+#13#10+#13#10;
    TLibUpdateForm_actUpdateRun_OnExecute(Sender);
  end;
  //============================================================================
  function TLibUpdateForm_Create(const aModalForm:Boolean):TForm;
  var
    lkMemo   :TMemo;
    lkToolBar:TxcGradientPanelVista;
    lkAct    :TCustomAction;
  begin
    Result := TCommonForm_Create(mgs_library_updates_caption, mgs_library_updates_caption,'PEAKTOP:IMG/CONFIG/UPDATE0016X0016.ICO',
      'PEAKTOP:IMG/CONFIG/UPDATE0032X0032.BMP', aModalForm, @TCommonForm_OnClose);
    with TxcGradientPanelVista(Result.FindComponent(obj_name_common_topbar)) do
      begin
      StyleManager          := nil;
      Colors.HotTrack       := $BFFF80 or $A0A0A0;
      Colors.HotTrackBorder := $BFFF80;
      end;  
    lkToolBar := CreateTxcGradientPanelVistaToolBar(Result, Result, 'ToolBar', '', alTop, 0,200,24,600, xbsRaized);  
    lkMemo := CreateTMemo(Result, Result, obj_name_common_memo, 0, 200, 200, 200, alClient);
    with lkMemo do
      begin
      Color     := Amunhotep.MainForm.StyleManager.Colors.Border;
      Font.Color:= Amunhotep.MainForm.StyleManager.Colors.Window;
      Text      := '';
      Font.Size := 8;
      Font.Name := 'Fixedsys';
      Font.Style:= [];
      ScrollBars:= ssBoth;
      WordWrap  := true;
      end; 
    lkAct := TCommonForm_CreateAction2(Result,obj_name_act_updatever,'Версия'  ,'Проверить текущую версию библиотеки кода','Ctrl+F5',0,170,@TLibUpdateForm_actUpdateVer_OnExecute,nil,lkToolBar, 60,nil);
    TLibUpdateForm_actUpdateVer_OnExecute(lkAct);
    lkAct := TCommonForm_CreateAction2(Result,obj_name_act_updateclr,'Очистить','Очистить кэш библиотеки кода'            ,'Ctrl+C' ,0, 67,@TLibUpdateForm_actUpdateClr_OnExecute,nil,lkToolBar, 80,nil);
    lkAct := TCommonForm_CreateAction2(Result,obj_name_act_updaterun,'Обновить','Обновить библиотеку кода из репозитария' ,'F9'     ,0, 93,@TLibUpdateForm_actUpdateRun_OnExecute,nil,lkToolBar, 80,nil);
    if FileExists(StrAbsolutePath('../conf/amunhotep-peaktop.conf', Amunhotep.ExeName))then
    lkAct := TCommonForm_CreateAction2(Result,'actCommitSVN'        ,'Commit'  ,'Commit SVN'                              ,''       ,0,143,@TLibUpdateForm_actCommitSVN_OnExecute,nil,lkToolBar, 60,nil);
  end;
