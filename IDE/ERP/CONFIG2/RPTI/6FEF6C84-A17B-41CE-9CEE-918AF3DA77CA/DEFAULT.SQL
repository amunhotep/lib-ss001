EXECUTE BLOCK (
  Q_DATE_FROM   TYPE OF COLUMN TABL$J_4.DATE_COMMIT = ?DATE_FROM
 ,Q_DATE_TO     TYPE OF COLUMN TABL$J_4.DATE_COMMIT = ?DATE_TO
 ,Q_FIRM_IDS    TYPE OF COLUMN TABL$J_4.DOCSTR      = ?FIRM_IDS
 ,Q_FILIAL_IDS  TYPE OF COLUMN TABL$J_4.DOCSTR      = ?FILIAL_IDS
)RETURNS (
   DATE_FROM       TYPE OF COLUMN TABL$J_4.DATE_COMMIT
  ,DATE_TO         TYPE OF COLUMN TABL$J_4.DATE_COMMIT
  ,FIRM_IDS        TYPE OF COLUMN TABL$J_4.DOCSTR
  ,FILIAL_IDS      TYPE OF COLUMN TABL$J_4.DOCSTR
  ,FILIAL_ID       TYPE OF COLUMN TABL$J_4.FILIAL_ID
  ,FILIAL_NAME     TYPE OF COLUMN TABL$R_FILIALS.NAME
  ,FILIAL_NAMEPL   TYPE OF COLUMN TABL$R_FILIALS.NAME
  ,PLATENO         TYPE OF COLUMN TABL$J_1000014.PLATENO
  ,DOCCNT          TYPE OF COLUMN TABL$J_4.DOCSUM
  ,GEUSTQUANT      TYPE OF COLUMN TABL$J_4.DOCSUM
  ,GEUSTQUANTAVG   TYPE OF COLUMN TABL$J_4.DOCSUM
  ,DOCSUM          TYPE OF COLUMN TABL$J_4.DOCSUM
  ,DOCSUMAVG       TYPE OF COLUMN TABL$J_4.DOCSUM
  ,DOCSUMAVGGUEST  TYPE OF COLUMN TABL$J_4.DOCSUM
  ,DOCCNT_         TYPE OF COLUMN TABL$J_4.DOCSUM
  ,GEUSTQUANT_     TYPE OF COLUMN TABL$J_4.DOCSUM
  ,GEUSTQUANTAVG_  TYPE OF COLUMN TABL$J_4.DOCSUM
  ,DOCSUM_         TYPE OF COLUMN TABL$J_4.DOCSUM
  ,DOCSUMAVG_      TYPE OF COLUMN TABL$J_4.DOCSUM
  ,DOCSUMAVGGUEST_ TYPE OF COLUMN TABL$J_4.DOCSUM
)AS
BEGIN
  DATE_FROM  = :Q_DATE_FROM;
  DATE_TO    = :Q_DATE_TO;
  FIRM_IDS   = :Q_FIRM_IDS;
  FILIAL_IDS = :Q_FILIAL_IDS;

  SELECT COUNT(JF.ID),SUM(JB.GUESTQUANT), SUM(JF.DOCSUM)
  FROM   TABL$J_4 JF, TABL$J_1000014 JB
  WHERE  (JF.DATE_COMMIT BETWEEN :DATE_FROM AND :DATE_TO)
    AND  (JF.TYPE_ID = 1000075)
    AND  (JF.FLAG_COMMIT = 1)
    AND  (:FIRM_IDS   CONTAINING '~'||JF.FIRM_ID||'~'  )
    AND  (:FILIAL_IDS CONTAINING '~'||JF.FILIAL_ID||'~')
    AND  (JB.J_ID = JF.ID)
  INTO   :DOCCNT_, :GEUSTQUANT_, :DOCSUM_;
  GEUSTQUANTAVG_  = 0;
  DOCSUMAVG_      = 0;
  DOCSUMAVGGUEST_ = 0;
  IF(:DOCCNT_ <> 0)THEN
    BEGIN
    GEUSTQUANTAVG_  = :GEUSTQUANT_ / :DOCCNT_;
    DOCSUMAVG_      = :DOCSUM_     / :DOCCNT_;
    END
  IF(:GEUSTQUANT_ <> 0)THEN  
    BEGIN
    DOCSUMAVGGUEST_ = :DOCSUM_    / :GEUSTQUANT_;
    END

  FOR
    SELECT JF.FILIAL_ID, COALESCE(JB.PLATENO,0), COUNT(JF.ID),SUM(JB.GUESTQUANT), SUM(JF.DOCSUM)
    FROM   TABL$J_4 JF, TABL$J_1000014 JB
    WHERE  (JF.DATE_COMMIT BETWEEN :DATE_FROM AND :DATE_TO)
      AND  (JF.TYPE_ID = 1000075)
      AND  (JF.FLAG_COMMIT = 1)
      AND  (:FIRM_IDS   CONTAINING '~'||JF.FIRM_ID||'~'  )
      AND  (:FILIAL_IDS CONTAINING '~'||JF.FILIAL_ID||'~')
      AND  (JB.J_ID = JF.ID)
    GROUP BY 1,2
    INTO   :FILIAL_ID, :PLATENO, :DOCCNT, :GEUSTQUANT, :DOCSUM
  DO
    BEGIN
    SELECT FIRST 1 F.NAME FROM TABL$R_FILIALS F WHERE (F.ID = :FILIAL_ID)INTO :FILIAL_NAME;
    GEUSTQUANTAVG  = 0;
    DOCSUMAVG      = 0;
    DOCSUMAVGGUEST = 0;
    IF(:DOCCNT <> 0)THEN
      BEGIN
      GEUSTQUANTAVG  = :GEUSTQUANT / :DOCCNT;
      DOCSUMAVG      = :DOCSUM     / :DOCCNT;
      END
    IF(:GEUSTQUANT <> 0)THEN  
      BEGIN
      DOCSUMAVGGUEST = :DOCSUM     / :GEUSTQUANT;
      END
    FILIAL_NAMEPL = :FILIAL_NAME||' ���� '||:PLATENO;  
    SUSPEND;
    END
END
