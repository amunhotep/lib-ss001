{$DEFINE PEAKTOP:IDE/ERP/DBO/VERSION/UTILS.INC}
{$IFNDEF PEAKTOP:GLOBAL_VARIABLES.INC} {$I PEAKTOP:GLOBAL_VARIABLES.INC} {$ENDIF}
  //=============== PEAKTOP:IDE/ERP/DBO/VERSION/UTILS.INC ======================
  function Update_ExecSQLParams(const aTransactionParams:array of string; const aSQL:string; const aParamNames, aParamValues:array of string):Boolean;
  var
    i    :Integer;
    lkTr :TxFBTransaction;
    lkQr :TxFBQuery;
  begin
    Result := false;
    if not FBDataBase.Connected then exit;
    lkTr := TxFBTransaction.Create(nil);
    with lkTr do
      begin
      DefaultDataBase := FBDataBase;
      with Params do
        begin
        Clear;
        if(Length(aTransactionParams) > 0)then
          for i:=0 to Length(aTransactionParams)-1 do
            Add(aTransactionParams[i])
         else
          begin
          Add('isc_tpb_read_committed');
          Add('isc_tpb_write');
          Add('isc_tpb_rec_version');
          Add('isc_tpb_nowait');
          end;   
        end;
      end;
    lkQr := TxFBQuery.Create(lkTr);
    with lkQR do
      begin
      Transaction := lkTr;
      SQL.Text := aSQL;
      if(Length(aParamNames) > 0)then
        begin
        Prepare;
        for i:=0 to Length(aParamNames)-1 do
          ParamByName(aParamNames[i]).AsString := aParamValues[i]; 
        end;
      try
        ExecSQL;
      except
        if lkTr.InTransaction then
          lkTr.Rollback;
        lkTr.Free;
        exit;    
      end;  
      end;
    if lkTr.InTransaction then
      lkTr.Commit;
    lkTr.Free;    
    Result := true;
  end;
  //============================================================================
  function Update_ExecSQLFromFile(const FileName:string):Boolean;
  var
    lkFN  :string;
    lkSQL :string;
  begin
    Result := False;
    lkFN := PrepareAbsoleteFileName(FileName);
    if not FileExists(lkFN) then
      begin
      Dialogs.MessageDlg('Error happened while opening file: '+#13#10+lkFN, mtWarning,[mbOK]);
      exit;
      end;
    lkSQL := StringsLoadFromFile(lkFN);
    if(StrTrimAll(lkSQL) = '')then
      begin
      Dialogs.MessageDlg('SQL-file is empty: '+#13#10+lkFN, mtWarning,[mbOK]);
      exit;
      end;
    Result := Update_ExecSQLParams(
       ['isc_tpb_write', 'isc_tpb_consistency']
      ,lkSQL
      ,[]
      ,[]
    );  
  end;
