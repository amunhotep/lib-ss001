EXECUTE BLOCK (
   Q_DATEFROM             TYPE OF COLUMN TABL$J_4.DATE_COMMIT = ?DATE_FROM
  ,Q_DATETO               TYPE OF COLUMN TABL$J_4.DATE_COMMIT = ?DATE_TO
  ,Q_FIRM_IDS             TYPE OF COLUMN TABL$J_4.DOCSTR      = ?FIRM_IDS
  ,Q_PLACE_IDS            TYPE OF COLUMN TABL$J_4.DOCSTR      = ?PLACES_IDS
)RETURNS(
   DATEFROM               TYPE OF COLUMN TABL$J_4.DATE_COMMIT
  ,DATETO                 TYPE OF COLUMN TABL$J_4.DATE_COMMIT
  ,PLACE_ID               TYPE OF COLUMN TABL$R_PLACES.ID
  ,PLACE_NAME             TYPE OF COLUMN TABL$R_PLACES.NAME
  ,PLACE_ACC_ID           TYPE OF COLUMN TABL$R_PLACES.ACC_ID
  ,FIRM_ID                TYPE OF COLUMN TABL$J_4.FIRM_ID
  ,FIRM_NAME              TYPE OF COLUMN TABL$R_FIRMS.NAME
  ,TMC_GROUP_ID           TYPE OF COLUMN TABL$R_TMC_GROUP.ID
  ,TMC_GROUP_NAME         TYPE OF COLUMN TABL$R_TMC_GROUP.NAME
  ,TMC_GROUP_PATH         TYPE OF COLUMN TABL$R_TMC_GROUP.PATH
  ,TMC_ID                 TYPE OF COLUMN TABL$R_TMC.ID
  ,TMC_NAME               TYPE OF COLUMN TABL$R_TMC.NAME
  ,TMC_ARTICLE            TYPE OF COLUMN TABL$R_TMC.ARTICLE
  ,TMC_BARCODE            TYPE OF COLUMN TABL$R_TMC.BARCODE
  ,TMC_COUNTRY_ID         TYPE OF COLUMN TABL$R_TMC.COUNTRY_ID
  ,VALUTE                 TYPE OF COLUMN TABL$R_COUNTRIES.VALUTE
  ,TMC_PROBE              TYPE OF COLUMN TABL$R_TMC.PROBE
  ,TMC_LGTH               TYPE OF COLUMN TABL$R_TMC.LGTH
  ,TMC_DIAM               TYPE OF COLUMN TABL$R_TMC.DIAM
  ,TMC_MASSA              TYPE OF COLUMN TABL$R_TMC.MASSA_NETTO
  ,TMC_MASSA_INS          TYPE OF COLUMN TABL$R_TMC.MASSA_NETTO
  ,TMC_MASSA_NETTO        TYPE OF COLUMN TABL$R_TMC.MASSA_NETTO
  ,TMC_MASSA_             TYPE OF COLUMN TABL$R_TMC.MASSA
  ,TMC_MASSA_INS_         TYPE OF COLUMN TABL$R_TMC.MASSA
  ,TMC_MASSA_NETTO_       TYPE OF COLUMN TABL$R_TMC.MASSA_NETTO
  ,PRICE                  TYPE OF COLUMN TABL$P_TMC_QUANT.QUANT
  ,QUANT                  TYPE OF COLUMN TABL$P_TMC_QUANT.QUANT
  ,TOTAL                  TYPE OF COLUMN TABL$P_TMC_QUANT.QUANT
  ,PRICE_SAVE             TYPE OF COLUMN TABL$P_TMC_QUANT.QUANT
  ,TOTAL_SAVE             TYPE OF COLUMN TABL$P_TMC_QUANT.QUANT
  ,AMOUNT                 TYPE OF COLUMN TABL$P_TMC_QUANT.QUANT
  ,AMOUNTPC               TYPE OF COLUMN TABL$P_TMC_QUANT.QUANT
  ,TMC_MASSAG             TYPE OF COLUMN TABL$R_TMC.MASSA
  ,TMC_MASSA_INSG         TYPE OF COLUMN TABL$R_TMC.MASSA
  ,TMC_MASSA_NETTOG       TYPE OF COLUMN TABL$R_TMC.MASSA_NETTO
  ,QUANTG                 TYPE OF COLUMN TABL$P_TMC_QUANT.QUANT
  ,TOTALG                 TYPE OF COLUMN TABL$P_TMC_QUANT.QUANT
  ,TOTAL_SAVEG            TYPE OF COLUMN TABL$P_TMC_QUANT.QUANT
  ,AMOUNTG                TYPE OF COLUMN TABL$P_TMC_QUANT.QUANT
  ,CS_ID                  TYPE OF COLUMN TABL$J_1000014.CS_ID
  ,CS_NAME                TYPE OF COLUMN TABL$R_CS.NAME
  ,J_1000028_ID           TYPE OF COLUMN TABL$J_4.ID
  ,J_1000028_DOCNUMBERSTR TYPE OF COLUMN TABL$J_4.DOCNUMBERSTR
  ,J_1000028_DATE_COMMIT  TYPE OF COLUMN TABL$J_4.DATE_COMMIT
)AS
  DECLARE VARIABLE P_TMC_GROUP_ID TYPE OF COLUMN TABL$R_TMC.TMC_GROUP_ID;
  DECLARE VARIABLE P_DATE         TYPE OF COLUMN TABL$J_4.DATE_COMMIT;
BEGIN
  DATEFROM = :Q_DATEFROM;
  DATETO   = :Q_DATETO;
  TMC_MASSAG       = 0;
  TMC_MASSA_INSG   = 0;
  TMC_MASSA_NETTOG = 0;
  QUANTG           = 0;
  TOTALG           = 0;
  TOTAL_SAVEG      = 0;
  AMOUNTG          = 0;
  P_TMC_GROUP_ID   = 0;
  FOR
    SELECT TM.FIRM_ID, (SELECT FIRST 1 F.NAME FROM TABL$R_FIRMS F WHERE(F.ID = TM.FIRM_ID)) AS FIRM_NAME
          ,TM.PLACE_ID, (SELECT FIRST 1 P.NAME FROM TABL$R_PLACES P WHERE(P.ID = TM.PLACE_ID)) AS PLACE_NAME
          ,(SELECT FIRST 1 P.ACC_ID FROM TABL$R_PLACES P WHERE(P.ID = TM.PLACE_ID)) AS PLACE_ACC_ID
          ,TM.TMC_ID, COALESCE(TM.QUANT,0), COALESCE(TM.PRICE_TMC,0)
          ,TMC.NAME, TMC.ARTICLE, TMC.BARCODE, TMC.COUNTRY_ID, TMC.PROBE
          ,(SELECT FIRST 1 C.VALUTE FROM TABL$R_COUNTRIES C WHERE (C.ID = TMC.COUNTRY_ID)) AS VALUTE
          ,TMC.MASSA, (TMC.MASSA - TMC.MASSA_NETTO) AS MASSA_INS, TMC.MASSA_NETTO
          ,TMC.LGTH, TMC.DIAM, TMC.TMC_GROUP_ID, TG.NAME AS TMC_GROUP_NAME
          ,TM.J_ID, J4.DOCNUMBERSTR, J4.DATE_COMMIT
          ,J14.CS_ID, CS.NAME
    FROM   TABL$P_TMC_QUANT_MOVE TM, TABL$R_TMC TMC, TABL$R_TMC_GROUP TG, TABL$J_4 J4
          ,TABL$J_1000014 J14, TABL$R_CS CS
    WHERE  (:Q_FIRM_IDS  CONTAINING '~'||TM.FIRM_ID||'~' )
      AND  (:Q_PLACE_IDS CONTAINING '~'||TM.PLACE_ID||'~' )
      AND  (TM.FLAG_DEBET = 0)
      AND  (TM.DATE_COMMIT BETWEEN :DATEFROM AND :DATETO)
      AND  (TMC.ID = TM.TMC_ID)
      AND  (TG.ID = TMC.TMC_GROUP_ID)
      AND  (J4.ID = TM.J_ID)
      AND  (J4.TYPE_ID = 1000028)
      AND  (J14.J_ID = J4.ID)
      AND  (CS.ID = J14.CS_ID)
    ORDER BY TG.NAME, TMC.COUNTRY_ID, TMC.PROBE
    INTO   :FIRM_ID, :FIRM_NAME, :PLACE_ID, :PLACE_NAME, :PLACE_ACC_ID
          ,:TMC_ID, :QUANT, :PRICE
          ,:TMC_NAME, :TMC_ARTICLE, :TMC_BARCODE, :TMC_COUNTRY_ID, :TMC_PROBE, :VALUTE
          ,:TMC_MASSA, :TMC_MASSA_INS, :TMC_MASSA_NETTO, :TMC_LGTH, :TMC_DIAM
          ,:TMC_GROUP_ID, :TMC_GROUP_NAME
          ,:J_1000028_ID, :J_1000028_DOCNUMBERSTR, :J_1000028_DATE_COMMIT
          ,:CS_ID, :CS_NAME
  DO
    BEGIN
    TMC_MASSA_       = :TMC_MASSA       * :QUANT;
    TMC_MASSA_INS_   = :TMC_MASSA_INS   * :QUANT;
    TMC_MASSA_NETTO_ = :TMC_MASSA_NETTO * :QUANT;
    TOTAL            = :PRICE           * :QUANT;
    SELECT MAX(P.VALUE_DATE)
    FROM   TABL$R_TMC_P P
    WHERE  (P.TMC_ID = :TMC_ID)
      AND  (P.VALUE_DATE < :J_1000028_DATE_COMMIT)
    INTO   :P_DATE;

    SELECT FIRST 1 COALESCE(P.PRICE, :PRICE)
    FROM   TABL$R_TMC_P P
    WHERE  (P.TMC_ID = :TMC_ID)
      AND  (P.VALUE_DATE = :P_DATE)
    INTO   :PRICE_SAVE;

    TOTAL_SAVE = :PRICE_SAVE * :QUANT;
    AMOUNT     = :TOTAL_SAVE - :TOTAL;
    IF(:TOTAL_SAVE <> 0)THEN
      AMOUNTPC = ((:AMOUNT * 100) / :TOTAL_SAVE);
     ELSE
      AMOUNTPC = 0;

    IF(:TMC_GROUP_ID <> :P_TMC_GROUP_ID)THEN
      BEGIN
      TMC_MASSAG       = 0;
      TMC_MASSA_INSG   = 0;
      TMC_MASSA_NETTOG = 0;
      QUANTG           = 0;
      TOTALG           = 0;
      TOTAL_SAVEG      = 0;
      AMOUNTG          = 0;
      P_TMC_GROUP_ID   = :TMC_GROUP_ID;
      END
    TMC_MASSAG       = :TMC_MASSAG       + :TMC_MASSA_;
    TMC_MASSA_INSG   = :TMC_MASSA_INSG   + :TMC_MASSA_INS_;
    TMC_MASSA_NETTOG = :TMC_MASSA_NETTOG + :TMC_MASSA_NETTO_;
    TOTAL_SAVEG      = :TOTAL_SAVEG      + :TOTAL_SAVE;
    AMOUNTG          = :AMOUNTG          + :AMOUNT;
    QUANTG           = :QUANTG           + :QUANT;
    TOTALG           = :TOTALG           + :TOTAL;

    SUSPEND;
    END
END
