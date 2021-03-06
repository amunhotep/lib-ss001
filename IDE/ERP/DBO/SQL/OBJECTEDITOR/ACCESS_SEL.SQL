EXECUTE BLOCK (
   Q_OBJ_ID    TYPE OF COLUMN TABL$_TB.ID           = ?OBJ_ID
  ,Q_OBJ_TYPE  TYPE OF COLUMN TABL$_TB.BASE_TYPE_ID = ?OBJ_TYPE
)RETURNS (
   OBJ_ID      TYPE OF COLUMN TABL$_TB.ID
  ,OBJ_TYPE    TYPE OF COLUMN TABL$_TB.BASE_TYPE_ID
  ,IMGIDX      TYPE OF COLUMN TABL$_TB.DLG_HEIGHT
  ,TB_ID       TYPE OF COLUMN TABL$_TB.ID
  ,ROLE_ID     TYPE OF COLUMN TABL$_USERS_ROLES.ID
  ,ROLE_NAME   TYPE OF COLUMN TABL$_USERS_ROLES.NAME
  ,ACCSS       TYPE OF COLUMN TABL$_TB.ACCSS
  ,ACCSS_INS   TYPE OF COLUMN TABL$_TB.ACCSS_INS
  ,ACCSS_UPD   TYPE OF COLUMN TABL$_TB.ACCSS_UPD
  ,ACCSS_DEL   TYPE OF COLUMN TABL$_TB.ACCSS_DEL
  ,ACCSS_PRINT TYPE OF COLUMN TABL$_TB.ACCSS_PRINT
  ,FLAG_SEL    TYPE OF COLUMN TABL$_TB.FLAG_DELETE
  ,FLAG_INS    TYPE OF COLUMN TABL$_TB.FLAG_DELETE
  ,FLAG_UPD    TYPE OF COLUMN TABL$_TB.FLAG_DELETE
  ,FLAG_DEL    TYPE OF COLUMN TABL$_TB.FLAG_DELETE
  ,FLAG_PRINT  TYPE OF COLUMN TABL$_TB.FLAG_DELETE
)
AS
BEGIN
  OBJ_ID   = :Q_OBJ_ID;
  OBJ_TYPE = :Q_OBJ_TYPE;
  IMGIDX   = 0;
  SELECT FIRST 1 T.ID, T.ACCSS, T.ACCSS_INS, T.ACCSS_UPD, T.ACCSS_DEL, T.ACCSS_PRINT
  FROM   TABL$_TB T
  WHERE  (T.ID = :OBJ_ID)
  INTO   :TB_ID, :ACCSS, :ACCSS_INS, :ACCSS_UPD, :ACCSS_DEL, :ACCSS_PRINT;
  FOR
    SELECT R.ID, R.NAME
    FROM   TABL$_USERS_ROLES R
    ORDER BY R.ID
    INTO   :ROLE_ID, :ROLE_NAME
  DO
    BEGIN
    IF(:ACCSS       CONTAINING '~'||:ROLE_ID||'~')THEN FLAG_SEL   = 1; ELSE FLAG_SEL   = 0;
    IF(:ACCSS_INS   CONTAINING '~'||:ROLE_ID||'~')THEN FLAG_INS   = 1; ELSE FLAG_INS   = 0;
    IF(:ACCSS_UPD   CONTAINING '~'||:ROLE_ID||'~')THEN FLAG_UPD   = 1; ELSE FLAG_UPD   = 0;
    IF(:ACCSS_DEL   CONTAINING '~'||:ROLE_ID||'~')THEN FLAG_DEL   = 1; ELSE FLAG_DEL   = 0;
    IF(:ACCSS_PRINT CONTAINING '~'||:ROLE_ID||'~')THEN FLAG_PRINT = 1; ELSE FLAG_PRINT = 0;
    SUSPEND;
    END
END
