{$DEFINE PEAKTOP:IDE/ERP/DBO/REF/FORMREF_LOADCHILDS.INC}
{$IFNDEF PEAKTOP:OBJ/TSPLITTER.INC}                           {$I PEAKTOP:OBJ/TSPLITTER.INC}               {$ENDIF}
{$IFNDEF PEAKTOP:IDE/ERP/DBO/$COMMON/FORMERP_HEADER.INC}      {$I ../$COMMON/FORMERP_HEADER.INC}           {$ENDIF}
{$IFNDEF PEAKTOP:IDE/ERP/DBO/DBGRID/ACTIONS.INC}              {$I ../DBGRID/ACTIONS.INC}                   {$ENDIF}
{$IFNDEF PEAKTOP:IDE/ERP/DBO/DBGRID/GETCELLPARAMS_REF.INC}    {$I ../DBGRID/GETCELLPARAMS_REF.INC}         {$ENDIF}
{$IFNDEF PEAKTOP:IDE/ERP/DBO/REF/FORMREF_LOADCOLUMNS.INC}     {$I ./FORMREF_LOADCOLUMNS.INC}               {$ENDIF}
  //============= PEAKTOP:IDE/ERP/DBO/REF/FORMREF$LOADCHILDS.INC ===============
  procedure TERPFormItem_DataSetChild_OnNewRecord(DataSet :TDataSet);
  var
    i      :Integer;
    lkStr  :string;
    lkOwner:TComponent;
    lkDBG  :TDBGridEh;
  begin
    with TxFBDataSet(DataSet) do
      begin
      for i:=0 to Fields.Count-1 do
        begin
        if( (Fields[i].FieldName = ERP_DATASET_DBKEY) or (Fields[i].FieldName = 'ID') )then
          try Fields[i].AsString := '-1'; except end;
        if(Fields[i].FieldName = GeneratorField.Field)then
          if GetField(FBDataBase, 'RDB$DATABASE', 'GEN_ID('+GeneratorField.Generator+', 0)', '', lkStr)then
            try Fields[i].AsString := IntToStr(StrToInt(lkStr) + GeneratorField.IncrementBy); except end;
        end;
      if( (SQLFilter.Text <> '') and (DataSource <> nil) )then
        FieldByName(SQLFilter.Names[0]).AsString := DataSource.DataSet.FieldByName(SQLFilter.Values[SQLFilter.Names[0]]).AsString;
      end;
    lkDBG := nil;
    lkOwner := TERPForm_GetOwner(DataSet);
    if(lkOwner <> nil)then
      lkDBG := TDBGridEh(lkOwner.FindComponent(obj_name_erp_data_dbgdata+DataSet.Name));
    if(lkDBG = nil)then exit;  
    TERPForm_DBGrid_FillValuesGrid(DataSet, nil, lkDBG);
  end;
  //============================================================================
  procedure TERPFormItem_LoadChilds(aQr:TxFBQuery; aOwner :TComponent; aParentDataSource:TDataSource; const aProcessBlob:Boolean);
  var
    i         :Integer;
    lkCMP     :TComponent;
    lkPC      :TxcPageControlEx;
    lkPCBLOB  :TxcPageControlEx;
    lkSplt    :TSplitter;
    lkTS      :TxcTabSheet;
    lkQr      :TxFBQuery;
    lkToolBar :TxcGradientPanelVista;
    lkDTS          :TxFBDataSet;
    lkDS           :TDataSource;
    lkDBG          :TDBGridEh;
    lkObjName      :string;
    lkSQLSelect    :string;
    lkSQLRefresh   :string; 
    lkSQLInsert    :string; 
    lkSQLUpdate    :string; 
    lkSQLDelete    :string;
    lkSelectFilter :string;
    lkCreateParams :TFormERPParams;
  begin
    lkCMP:= aOwner.FindComponent(obj_name_erp_pagecontroldetail);
    if(lkCMP = nil)then exit;
    lkQr := TxFBQuery.Create(nil);
    with lkQr do
      begin
      Database    := aQr.DataBase;
      Transaction := aQr.Transaction;
      end;
    lkPC := TxcPageControlEx(lkCMP);
    aQr.First;
    i := 0;
    while not aQr.EOF do
      begin
      //WriteLn(aQr.FieldByName('TB_CHILD_ID').AsString);
      lkObjName := StrReplaceStr(aQr.FieldByName('TB_CHILD_ID').AsString,'$','_')+RandomHexStr(4);
      lkTS      := CreateTxcTabSheet(lkPC, 'ts'+lkObjName, aQr.FieldByName('TB_CHILD_NAME').AsString, ERP_ObjectImageIndex(aQr.FieldByName('TB_CHILD_TYPE').AsInteger), ERP_ObjectColor(aQr.FieldByName('TB_CHILD_TYPE').AsInteger));
      lkToolBar := CreateTxcGradientPanelVistaToolBar(lkTS,lkTS,obj_name_erp_toolbar,'',alTop, 0,0,TERPForm_DefaultToolButtonWidth+2, 200,xbsNone);
      lkDTS := TxFBDataSet.Create(aParentDataSource.DataSet);
      with lkDTS do
        begin
        Name        := obj_name_erp_data_dtsdata+'_'+lkObjName;
        Transaction := TERPForm_DataSetMain(aOwner).Transaction;
        DataSource  := aParentDataSource;
        with GeneratorField do
          begin
          ApplyEvent  := gamOnServer;
          Field       := aQr.FieldByName('TB_CHILD_GENR_FLD').AsString;
          Generator   := aQr.FieldByName('TB_CHILD_GENR').AsString;
          IncrementBy := 1;
          end;
        SQLFilter.Clear;
        SQLFilter.Add(aQr.FieldByName('INDEX_FIELDS').AsString+'='+aQr.FieldByName('KEY_FIELDS').AsString);
        OnNewRecord := @TERPFormItem_DataSetChild_OnNewRecord;
        end;
      lkDS := TDataSource.Create(lkDTS);
      with lkDS do
        begin
        Name := 'DataSource'+lkDTS.Name;
        DataSet := lkDTS;
        end;
      lkDBG := CreateTDBGridEh(aOwner,lkTS,lkDS,obj_name_erp_data_dbgdata+lkDTS.Name, 0,100,200,200,alClient);
      with lkDBG do
        begin
        HelpKeyWord := '0';
        with IndicatorTitle do
          begin
          UseGlobalMenu    := true;
          ShowDropDownSign := true;
          TitleButton      := true;
          end;
        Options      := Options   + [dgMultiSelect, dgAlwaysShowSelection];  
        OptionsEh    := OptionsEh + [dghFixed3D, dghFrozen3D, dghFooter3D, dghAutoSortMarking, dghMultiSortMarking,  
          dghIncSearch, dghHighlightFocus, dghRowHighlight, dghColumnResize, dghColumnMove, dghExtendVertLines];
        EditActions  := [geaSelectAllEh];
        EditActions  := EditActions + [geaCopyEh];
        AllowedOperations := [];
        SortLocal    := true;
        VTitleMargin := 4;
        OnGetCellParams := @ERP_DBGridEh_OnGetCellParams_Ref;
        Columns.Clear;
        end; 
      lkCreateParams := [erpGrid, erpGridNavigation];
      if(aQr.FieldByName('TB_CHILD_ACCSPRN').AsInteger > 0)then
        lkCreateParams := lkCreateParams + [erpGridExport]; 
      if(aQr.FieldByName('TB_CHILD_ACCSEDT').AsInteger > 0)then
        begin
        lkCreateParams := lkCreateParams + [erpGridEdit, erpGridEditPostCancel];
        if(aQr.FieldByName('TB_CHILD_ACCSINS').AsInteger > 0)then lkCreateParams := lkCreateParams + [erpGridEditIns, erpGridEditCreate];
        if(aQr.FieldByName('TB_CHILD_ACCSUPD').AsInteger > 0)then lkCreateParams := lkCreateParams + [erpGridEditUpd, erpGridEditEdt, erpGridEditMark, erpGridEditUpdGroup];
        if(aQr.FieldByName('TB_CHILD_ACCSDEL').AsInteger > 0)then lkCreateParams := lkCreateParams + [erpGridEditDel];
        end;
      ERP_DBGridEh_EdtActions_Create(lkDBG, lkCreateParams, lkToolBar, nil);
      ERP_DBGridEh_StdActions_Create(lkDBG, lkCreateParams, lkToolBar, nil);
      lkPCBLOB := nil;  
      if(aQr.FieldByName('TB_CHILD_HASBLOB').AsInteger > 0)then
        begin  
        lkPCBLOB := CreateTxcPageControlEx(aOwner,lkDBG.Parent,obj_name_erp_pagecontroldetail+'_'+lkObjName,0,0,200,320,alRight);
        lkSplt := CreateTSplitter(aOwner,lkPCBLOB.Parent,obj_name_erp_splitterdetail+'_'+lkObjName,alRight,24,0,200,4);
        end;
      TERPFormGrid_LoadDBGridColumns(aQr.FieldByName('TB_CHILD_ID').AsString, lkQr, lkDBG, lkPCBLOB, true, false, 
        aQr.FieldByName('INDEX_FIELDS').AsString, lkSQLSelect, lkSQLRefresh, lkSQLInsert, lkSQLUpdate, lkSQLDelete, lkSelectFilter);
//      if(aQr.FieldByName('TB_CHILD_TYPE').AsInteger = ERP_OBJECTTYPE_REF)then TERPForm_GridSettingsLoad(lkDBG, 'TMP:'+ERP_FILENAME_CFG_DIALOG_REF+aQr.FieldByName('TB_CHILD_ID').AsString+'.CFG');
//      if(aQr.FieldByName('TB_CHILD_TYPE').AsInteger = ERP_OBJECTTYPE_REG)then TERPForm_GridSettingsLoad(lkDBG, 'TMP:'+ERP_FILENAME_CFG_DIALOG_REG+aQr.FieldByName('TB_CHILD_ID').AsString+'.CFG');
      with lkDTS do
        begin 
        SelectSQL.Text  := lkSQLSelect+'  AND ('+ERP_SQL_TABLEPREFIX+'.'+aQr.FieldByName('INDEX_FIELDS').AsString+' = ?'+aQr.FieldByName('KEY_FIELDS').AsString+')';  
        RefreshSQL.Text := lkSQLRefresh;
        if(aQr.FieldByName('TB_CHILD_ACCSINS').AsInteger > 0)then 
          begin
          lkDBG.AllowedOperations := lkDBG.AllowedOperations + [alopInsertEh, alopAppendEh]; 
          InsertSQL.Text := lkSQLInsert;
          end; 
        if(aQr.FieldByName('TB_CHILD_ACCSUPD').AsInteger > 0)then 
          begin
          lkDBG.AllowedOperations := lkDBG.AllowedOperations + [alopUpdateEh]; 
          ModifySQL.Text := lkSQLUpdate;
          end; 
        if(aQr.FieldByName('TB_CHILD_ACCSDEL').AsInteger > 0)then 
          begin
          lkDBG.AllowedOperations := lkDBG.AllowedOperations + [alopDeleteEh]; 
          DeleteSQL.Text := lkSQLDelete;          
          end; 
        end;
      i := i + 1;
      aQr.Next;
      end;
    lkQr.Free;  
    TxcPageControlEx_ActivateDefaultPage(lkPC);    
  end;  
