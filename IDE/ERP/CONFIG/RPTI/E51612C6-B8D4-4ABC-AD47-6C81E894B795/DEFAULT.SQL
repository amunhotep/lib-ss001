EXECUTE BLOCK (
  Q_DATE_FROM    TYPE OF COLUMN TABL$J_4.DATE_COMMIT     = ?DATE_FROM
 ,Q_DATE_TO      TYPE OF COLUMN TABL$J_4.DATE_COMMIT     = ?DATE_TO
 ,Q_PAYSRC_ID    TYPE OF COLUMN TABL$J_1000016.PAYSRC_ID = ?PAYSRC_ID
)RETURNS (
  DATE_FROM      TYPE OF COLUMN TABL$J_4.DATE_COMMIT
 ,DATE_TO        TYPE OF COLUMN TABL$J_4.DATE_COMMIT
 ,DATE_COMMIT    TYPE OF COLUMN TABL$J_4.DATE_COMMIT
 ,PAYSRC_ID      TYPE OF COLUMN TABL$R_PAYSRC.ID
 ,PAYSRC_NAME    TYPE OF COLUMN TABL$R_PAYSRC.NAME
 ,FIRM_ID        TYPE OF COLUMN TABL$R_PAYSRC.FIRM_ID
 ,FILIAL_ID      TYPE OF COLUMN TABL$R_PAYSRC.FILIAL_ID
 ,FLAG_DEBET     TYPE OF COLUMN TABL$J_4.FLAG_COMMIT
 ,ACC_ID         TYPE OF COLUMN TABL$J_1000016.ACC_ID
 ,NAME           TYPE OF COLUMN TABL$J_4.DOCSTR
 ,SUBKONTO       TYPE OF COLUMN TABL$J_4.DOCSTR
 ,SALDO          TYPE OF COLUMN TABL$J_4.DOCSUM
 ,DEBET          TYPE OF COLUMN TABL$J_4.DOCSUM
 ,KREDIT         TYPE OF COLUMN TABL$J_4.DOCSUM
 ,J_ID           TYPE OF COLUMN TABL$J_4.ID
 ,J_TYPE_ID      TYPE OF COLUMN TABL$J_4.TYPE_ID
 ,J_TYPE_NAME    TYPE OF COLUMN TABL$_TB_TYPES.NAME
 ,J_DOCNUMBERSTR TYPE OF COLUMN TABL$J_4.DOCNUMBERSTR
)AS
  DECLARE VARIABLE P_DEBET   TYPE OF COLUMN TABL$J_4.DOCSUM;
  DECLARE VARIABLE P_KREDIT  TYPE OF COLUMN TABL$J_4.DOCSUM;
  DECLARE VARIABLE PS_DEBET  TYPE OF COLUMN TABL$J_4.DOCSUM;
  DECLARE VARIABLE PS_KREDIT TYPE OF COLUMN TABL$J_4.DOCSUM;
BEGIN
  DATE_FROM  = :Q_DATE_FROM;
  DATE_TO    = :Q_DATE_TO;
  PAYSRC_ID  = :Q_PAYSRC_ID; 
  SELECT FIRST 1 C.NAME, C.FIRM_ID, C.FILIAL_ID
  FROM   TABL$R_PAYSRC C
  WHERE  (C.ID = :PAYSRC_ID)
  INTO   :PAYSRC_NAME, :FIRM_ID, :FILIAL_ID;
  FLAG_DEBET     = -1;
  ACC_ID         = 0;
  NAME           = '';
  SUBKONTO       = :NAME;
  SALDO          = 0;
  DEBET          = 0;
  KREDIT         = 0;
  J_ID           = 0;
  J_TYPE_ID      = 0;
  J_TYPE_NAME    = '';
  J_DOCNUMBERSTR = '';
  DATE_COMMIT    = NULL;
  ------------------------------------------------------------------------------
  SELECT COALESCE(SUM(P.PAYSRC_VALUE),0)
  FROM   TABL$P_PAYSRC_MOVE P
  WHERE  (P.DATE_COMMIT < :DATE_FROM)
    AND  (P.PAYSRC_ID = :PAYSRC_ID)
    AND  (P.FLAG_DEBET = 1)
  INTO   :DEBET;
  SELECT COALESCE(SUM(P.PAYSRC_VALUE),0)
  FROM   TABL$P_PAYSRC_MOVE P
  WHERE  (P.DATE_COMMIT < :DATE_FROM)
    AND  (P.PAYSRC_ID = :PAYSRC_ID)
    AND  (P.FLAG_DEBET = 0)
  INTO   :KREDIT;
  SALDO = :KREDIT - :DEBET;
  DEBET  = 0;
  KREDIT = 0;
  IF(:SALDO < 0)THEN DEBET = (-1) * :SALDO; ELSE KREDIT = :SALDO;
  SALDO = 0;
  NAME = '������� �� ������� ���I���';
  SUBKONTO       = :NAME;
  SUSPEND;
  NAME = '';
  SUBKONTO       = :NAME;
  P_DEBET  = 0; PS_DEBET  = :DEBET;
  P_KREDIT = 0; PS_KREDIT = :KREDIT;
  FOR
    SELECT P.FLAG_DEBET, P.ACC_ID, P.SUBKONTO_NAME, P.PAYSRC_VALUE, P.J_ID, J.TYPE_ID, TT.NAME2, J.DOCNUMBERSTR, J.NAME, P.DATE_COMMIT
    FROM   TABL$P_PAYSRC_MOVE P, TABL$J_4 J, TABL$_TB_TYPES TT
    WHERE  (P.DATE_COMMIT BETWEEN :DATE_FROM AND :DATE_TO)
      AND  (P.PAYSRC_ID = :PAYSRC_ID)
      AND  (J.ID = P.J_ID)
      AND  (TT.ID = J.TYPE_ID)
    ORDER BY P.DATE_COMMIT, P.J_ID  
    INTO   :FLAG_DEBET, :ACC_ID, :SUBKONTO, :SALDO, :J_ID, :J_TYPE_ID, :J_TYPE_NAME, :J_DOCNUMBERSTR, :NAME, :DATE_COMMIT
  DO
    BEGIN
    DEBET  = 0;
    KREDIT = 0;
    IF(:FLAG_DEBET = 1)THEN
      BEGIN
      DEBET   = :SALDO;
      P_DEBET = :P_DEBET + :DEBET;
      END
     ELSE
      BEGIN
      KREDIT   = :SALDO;
      P_KREDIT = :P_KREDIT + :KREDIT;
      END
    SALDO = 0;
    SUSPEND;
    END
  FLAG_DEBET     = -1;
  NAME           = '��I� �� ���I��';
  ACC_ID         = 0;
  SUBKONTO       = :NAME;
  SALDO          = 0;
  DEBET          = :P_DEBET;
  KREDIT         = :P_KREDIT;
  J_ID           = 0;
  J_TYPE_ID      = 0;
  J_TYPE_NAME    = '';
  J_DOCNUMBERSTR = '';
  DATE_COMMIT    = NULL;
  SUSPEND;
  SALDO  = :PS_DEBET - :PS_KREDIT + :P_DEBET - :P_KREDIT;
  DEBET  = 0;
  KREDIT = 0;
  IF(:SALDO > 0)THEN DEBET = :SALDO; ELSE KREDIT = (-1) * :SALDO;
  SALDO    = 0;
  NAME     = '������� �� �I���� ���I���';
  SUBKONTO = :NAME;
  SUSPEND;
END
