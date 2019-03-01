{$DEFINE PEAKTOP:IDE/$COMMON/LIBRARYUPDATES_COMMIT.INC}
{$IFNDEF PEAKTOP:GLOBAL_VARIABLES.INC}                {$I PEAKTOP:GLOBAL_VARIABLES.INC}              {$ENDIF}
{$IFNDEF PEAKTOP:APPL/CONSOLE/CAPTURED_WINDOWS.INC}   {$I PEAKTOP:APPL/CONSOLE/CAPTURED_WINDOWS.INC} {$ENDIF}
  //================== PEAKTOP:IDE/$COMMON/LIBRARYUPDATES_COMMIT.INC ===========
  function AmunhotepStandardLibrary_CommitSVN:string;
  var
    i     :Integer;
    lkSL  :TStrings;
  begin
    StringsSaveToFile(PrepareAbsoleteFileName('PEAKTOP:VERSION.CONF'), '$Revision$  HASH:'+RandomHexStr(32));
    Result := FormatDateTime('dd.mm.yyyy hh:nn:ss',Now)+': Commiting SVN.'+#13#10;
    lkSL := TStringList.Create;
    lkSL.Text := 
      RunCaptured(Amunhotep.SourcePath, 'd:\appl\svn\bin\svn.exe', ' propset svn:keywords "Revision" VERSION.CONF')+#13#10+
      RunCaptured('d:\appl\svn\bin\', 'd:\appl\svn\bin\svn.exe', 
        ' commit d:\appl\peaktop\lib\amunhotep\ --message "'+FormatDateTime('yyyy.mm.dd hh:nn:ss', Now)+
        ' PEAKTOP" --non-interactive --no-auth-cache --username peaktop --password database')+#13#10+#13#10;
    for i:=0 to lkSL.Count-1 do
      lkSL[i] := #$20#$20#$20#$20#$20#$20#$20#$20+lkSL[i];
    Result := Result + lkSL.Text;
    lkSL.Free;  
  end;
