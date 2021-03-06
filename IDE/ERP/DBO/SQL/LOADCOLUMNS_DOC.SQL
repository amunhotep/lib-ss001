EXECUTE BLOCK (
   Q_TB_ID          TYPE OF COLUMN TABL$_TB.ID    = ?PQ_TB_ID
  ,Q_USER_VAR       TYPE OF COLUMN TABL$_USERS.ID = ?PQ_USER_VAR
)RETURNS ( 
   TB_ID            TYPE OF COLUMN TABL$_TB_COLS.TB_ID
  ,MAIN_BASE_TYPE_ID TYPE OF COLUMN TABL$_TB.BASE_TYPE_ID
  ,MAINTB_ID        TYPE OF COLUMN TABL$_TB_COLS.TB_ID
  ,MAIN_BUFFER_VAR  TYPE OF COLUMN TABL$_TB.BUFFER_VAR
  ,MAIN_SC_REF      TYPE OF COLUMN TABL$_TB.SC_REF
  ,MAIN_SC_REF_DLG  TYPE OF COLUMN TABL$_TB.SC_REF_DLG
  ,MAIN_SC_ITEM     TYPE OF COLUMN TABL$_TB.SC_ITEM
  ,MAIN_SC_ITEM_DLG TYPE OF COLUMN TABL$_TB.SC_ITEM_DLG
  ,ID               TYPE OF COLUMN TABL$_TB_COLS.ID 
  ,NAME             TYPE OF COLUMN TABL$_TB_COLS.NAME 
  ,HINT             TYPE OF COLUMN TABL$_TB_COLS.HINT 
  ,FLAG_DELETE      TYPE OF COLUMN TABL$_TB_COLS.FLAG_DELETE 
  ,FIELD_TYPE       TYPE OF COLUMN TABL$_TB_COLS.FIELD_TYPE 
  ,ALIGNMENT        TYPE OF COLUMN TABL$_TB_COLS.ALIGNMENT 
  ,FORMAT           TYPE OF COLUMN TABL$_TB_COLS.FORMAT 
  ,WIDTH            TYPE OF COLUMN TABL$_TB_COLS.WIDTH 
  ,COL_INDEX        TYPE OF COLUMN TABL$_TB_COLS.COL_INDEX 
  ,ORIENTATION      TYPE OF COLUMN TABL$_TB_COLS.ORIENTATION 
  ,PICK_LIST        TYPE OF COLUMN TABL$_TB_COLS.PICK_LIST 
  ,KEY_LIST         TYPE OF COLUMN TABL$_TB_COLS.KEY_LIST 
  ,REF_TABLE        TYPE OF COLUMN TABL$_TB_COLS.REF_TABLE 
  ,REF_FIELD        TYPE OF COLUMN TABL$_TB_COLS.REF_FIELD 
  ,REF_KEYFIELD     TYPE OF COLUMN TABL$_TB_COLS.REF_KEYFIELD 
  ,REF_WHERE        TYPE OF COLUMN TABL$_TB_COLS.REF_WHERE 
  ,REF_WHEREEDIT    TYPE OF COLUMN TABL$_TB_COLS.REF_WHEREEDIT 
  ,REF_BUFFER_VAR   TYPE OF COLUMN TABL$_TB_COLS.REF_FIELD 
  ,REF_SC_REF_DLG   TYPE OF COLUMN TABL$_TB_COLS.REF_WHERE 
  ,REF_SC_ITEM      TYPE OF COLUMN TABL$_TB.SC_ITEM
  ,REF_SC_ITEM_DLG  TYPE OF COLUMN TABL$_TB.SC_ITEM_DLG
  ,SUBKONTO_FIELD   TYPE OF COLUMN TABL$_TB_COLS.SUBKONTO_FIELD
  ,FLAG_FOOTER      TYPE OF COLUMN TABL$_TB_COLS.FLAG_FOOTER
  ,FOOTER_COUNT     TYPE OF COLUMN TABL$_TB_COLS.COL_INDEX
  ,FLAG_ADDITIONAL  TYPE OF COLUMN TABL$_TB_COLS.FLAG_ADDITIONAL 
  ,FLAG_PERIODICAL  TYPE OF COLUMN TABL$_TB_COLS.FLAG_PERIODICAL
  ,FLAG_GROUPBY     TYPE OF COLUMN TABL$_TB_COLS.FLAG_GROUPBY 
  ,CALC_SQL         TYPE OF COLUMN TABL$_TB_COLS.CALC_SQL       
  ,ACCS_FLAG_READ   TYPE OF COLUMN TABL$_TB_COLS.FLAG_DELETE 
  ,ACCS_FLAG_WRITE  TYPE OF COLUMN TABL$_TB_COLS.FLAG_DELETE 
  ,DEFAULT_VALUE    TYPE OF COLUMN TABL$_TB_COLS.DEFAULT_VALUE 
)AS 
BEGIN
  MAINTB_ID = :Q_TB_ID;
  SELECT FIRST 1 TBM.BASE_TYPE_ID, TBM.BUFFER_VAR, TBM.SC_REF, TBM.SC_REF_DLG, TBM.SC_ITEM, TBM.SC_ITEM_DLG
  FROM   TABL$_TB TBM 
  WHERE  (TBM.ID = :MAINTB_ID)
  INTO   :MAIN_BASE_TYPE_ID, :MAIN_BUFFER_VAR, :MAIN_SC_REF, :MAIN_SC_REF_DLG, :MAIN_SC_ITEM, :MAIN_SC_ITEM_DLG;

  FOR
    SELECT TC.TB_ID, TC.ID, TC.NAME, TC.HINT, TC.FLAG_DELETE, TC.FIELD_TYPE, TC.ALIGNMENT, 
           TC.FORMAT,TC.WIDTH,TC.COL_INDEX,TC.PICK_LIST,TC.KEY_LIST,TC.REF_TABLE, 
           TC.REF_FIELD,TC.REF_KEYFIELD,TC.REF_WHERE,TC.REF_WHEREEDIT,TC.FLAG_FOOTER, 
           TC.FLAG_ADDITIONAL, TC.FLAG_PERIODICAL, TC.DEFAULT_VALUE, TC.ORIENTATION,
           TC.CALC_SQL, TC.FLAG_GROUPBY, TC.SUBKONTO_FIELD 
    FROM   TABL$_TB TB, TABL$_TB_COLS TC 
    WHERE  (TB.ID  = :Q_TB_ID)
      AND  (TC.TB_ID = TB.ID)
    ORDER BY TC.FLAG_PERIODICAL, TC.COL_INDEX 
    INTO   :TB_ID, :ID, :NAME, :HINT, :FLAG_DELETE, :FIELD_TYPE, :ALIGNMENT, 
           :FORMAT, :WIDTH, :COL_INDEX, :PICK_LIST, :KEY_LIST, :REF_TABLE, 
           :REF_FIELD, :REF_KEYFIELD, :REF_WHERE, :REF_WHEREEDIT, :FLAG_FOOTER, 
           :FLAG_ADDITIONAL, :FLAG_PERIODICAL, :DEFAULT_VALUE, :ORIENTATION,
           :CALC_SQL, :FLAG_GROUPBY, :SUBKONTO_FIELD 
  DO 
/*  
    IF(EXISTS(
      SELECT R.RDB$FIELD_ID
      FROM   RDB$RELATION_FIELDS R
      WHERE  (TRIM(R.RDB$RELATION_NAME) = :Q_TB_ID)
        AND  (TRIM(R.RDB$FIELD_NAME)    = :ID)
    ) OR (TRIM(:CALC_SQL) <> ''))THEN
*/
      BEGIN 
      ACCS_FLAG_READ  = 0; 
      ACCS_FLAG_WRITE = 0; 
      FOOTER_COUNT    = 0;
      REF_BUFFER_VAR  = '' ; 
      REF_SC_REF_DLG  = '' ; 
      REF_SC_ITEM     = '';
      REF_SC_ITEM_DLG = '';
      IF(TRIM(:REF_TABLE) <> '')THEN 
        BEGIN 
        SELECT FIRST 1 TB1.BUFFER_VAR, TB1.SC_REF_DLG, TB1.SC_ITEM, TB1.SC_ITEM_DLG 
        FROM   TABL$_TB TB1 
        WHERE  (TB1.ID = :REF_TABLE) 
        INTO   :REF_BUFFER_VAR, :REF_SC_REF_DLG, :REF_SC_ITEM, :REF_SC_ITEM_DLG; 
        END 
      IF(:FLAG_FOOTER <> 0)THEN
        BEGIN
        SELECT COUNT(TF.ID)
        FROM   TABL$_TB_COLS_FOOTERS TF
        WHERE  (TF.TB_ID = :TB_ID)
          AND  (TF.TC_ID = :ID ) 
        INTO  :FOOTER_COUNT;
        END   
      ACCS_FLAG_READ  = 0;
      ACCS_FLAG_WRITE = 0;
      SELECT COALESCE(COUNT(TC.ID),0)
      FROM   TABL$_TB_COLS TC, TABL$_USERS_ACCS UA
      WHERE  (TC.TB_ID   = :TB_ID)
        AND  (TC.ID      = :ID)
        AND  (UA.USER_ID = RDB$GET_CONTEXT('USER_SESSION', :Q_USER_VAR))
        AND  (TC.ACCSS CONTAINING '~'||UA.ROLE_ID||'~')
      INTO   :ACCS_FLAG_READ;
      SELECT COALESCE(COUNT(TC.ID),0)
      FROM   TABL$_TB_COLS TC, TABL$_USERS_ACCS UA
      WHERE  (TC.TB_ID   = :TB_ID)
        AND  (TC.ID      = :ID)
        AND  (UA.USER_ID = RDB$GET_CONTEXT('USER_SESSION', :Q_USER_VAR))
        AND  (TC.ACCSSW CONTAINING '~'||UA.ROLE_ID||'~')
      INTO   :ACCS_FLAG_WRITE;
      SUSPEND; 
      END 
END
