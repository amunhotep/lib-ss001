EXECUTE BLOCK (
  Q_DATE_FROM TYPE OF COLUMN TABL$J_4.DATE_COMMIT = ?DATE_FROM
 ,Q_DATE_TO   TYPE OF COLUMN TABL$J_4.DATE_COMMIT = ?DATE_TO
 ,Q_FIRM_IDS  TYPE OF COLUMN TABL$J_4.DOCSTR      = ?FIRM_IDS
)RETURNS ( 
  DATE_FROM   TYPE OF COLUMN TABL$J_4.DATE_COMMIT
 ,DATE_TO     TYPE OF COLUMN TABL$J_4.DATE_COMMIT
 ,FIRM_IDS    TYPE OF COLUMN TABL$J_4.DOCSTR
 ,DATE_RPT    TYPE OF COLUMN TABL$J_4.DATE_COMMIT
 ,S0002       TYPE OF COLUMN TABL$J_4.DOCSUM
 ,S0003       TYPE OF COLUMN TABL$J_4.DOCSUM
 ,S0004       TYPE OF COLUMN TABL$J_4.NAME
 ,S0005       TYPE OF COLUMN TABL$J_4.DOCSUM
 ,S0006       TYPE OF COLUMN TABL$J_4.DOCSUM
 ,S0007       TYPE OF COLUMN TABL$J_4.NAME
 ,S0008       TYPE OF COLUMN TABL$J_4.DOCSUM
 ,S0009       TYPE OF COLUMN TABL$J_4.DOCSUM
 ,S0010       TYPE OF COLUMN TABL$J_4.DOCSUM
 ,S0011       TYPE OF COLUMN TABL$J_4.DOCSUM
 ,S0012       TYPE OF COLUMN TABL$J_4.DOCSUM
)AS
  DECLARE VARIABLE P_PLACE_IDS TYPE OF COLUMN TABL$J_4.DOCSTR;
  DECLARE VARIABLE P_TMC_ID    TYPE OF COLUMN TABL$R_TMC.ID;
  DECLARE VARIABLE P_PRICE_IN  TYPE OF COLUMN TABL$J_4.DOCSUM;
  DECLARE VARIABLE P_PRICE     TYPE OF COLUMN TABL$J_4.DOCSUM;
  DECLARE VARIABLE P_QUANT     TYPE OF COLUMN TABL$J_4.DOCSUM;
BEGIN 
  DATE_FROM = :Q_DATE_FROM;
  DATE_TO   = :Q_DATE_TO;
  FIRM_IDS  = :Q_FIRM_IDS;
  SELECT '~'||LIST(PL.ID, '~')||'~' FROM TABL$R_PLACES PL WHERE (PL.ACC_ID = 281)AND(PL.FILIAL_ID <> 0) INTO :P_PLACE_IDS;
  
  DATE_RPT  = :DATE_FROM;
  WHILE(:DATE_RPT < :DATE_TO)DO
    BEGIN
    S0004 = ''; S0007 = '';
    S0002 = 0; S0003 = 0; S0005 = 0; S0006 = 0;
    S0008 = 0; S0009 = 0; S0010 = 0; S0011 = 0;

    FOR
      SELECT Q.TMC_ID, SUM(Q.QUANT) AS QUANT
      FROM   (SELECT  TQ1.TMC_ID, TQ1.QUANT
              FROM    TABL$P_TMC_QUANT TQ1
              WHERE  (:FIRM_IDS  CONTAINING '~'||TQ1.FIRM_ID ||'~')
                AND  (:P_PLACE_IDS CONTAINING '~'||TQ1.PLACE_ID||'~')
              UNION ALL  -- DEBET
              SELECT  TQD.TMC_ID, ((-1) * TQD.QUANT) AS QUANT
              FROM    TABL$P_TMC_QUANT_MOVE TQD
              WHERE  (:FIRM_IDS  CONTAINING '~'||TQD.FIRM_ID ||'~')
                AND  (:P_PLACE_IDS CONTAINING '~'||TQD.PLACE_ID||'~')
                AND  (TQD.FLAG_DEBET = 1)
                AND  (TQD.DATE_COMMIT >= :DATE_RPT)
              UNION ALL  -- KREDIT
              SELECT  TQK.TMC_ID, TQK.QUANT
              FROM    TABL$P_TMC_QUANT_MOVE TQK
              WHERE  (:FIRM_IDS  CONTAINING '~'||TQK.FIRM_ID ||'~')
                AND  (:P_PLACE_IDS CONTAINING '~'||TQK.PLACE_ID||'~')
                AND  (TQK.FLAG_DEBET = 0)
                AND  (TQK.DATE_COMMIT >= :DATE_RPT)
              )Q
            GROUP BY 1
      INTO :P_TMC_ID, :P_QUANT
    DO
      BEGIN
      SELECT FIRST 1 COALESCE(D.PRICE,0)
      FROM   TABL$D_1000014 D, TABL$J_4 J
      WHERE  (D.TMC_ID = :P_TMC_ID)
        AND  (J.ID = D.J_ID)
        AND  (J.TYPE_ID = 1000026)
        AND  (J.FLAG_COMMIT = 1)
      INTO   :P_PRICE_IN;
      S0002 = :S0002 + (:P_QUANT * :P_PRICE_IN);        

      SELECT COALESCE(SUM(Q.QUANT),0)
      FROM   (SELECT  (TQD.QUANT) AS QUANT
              FROM    TABL$P_TMC_QUANT_MOVE TQD
              WHERE  (TQD.TMC_ID = :P_TMC_ID)
                AND  (TQD.DATE_COMMIT BETWEEN :DATE_RPT AND DATEADD(1 DAY TO :DATE_RPT))
                AND  (:FIRM_IDS  CONTAINING '~'||TQD.FIRM_ID ||'~')
                AND  (:P_PLACE_IDS CONTAINING '~'||TQD.PLACE_ID||'~')
                AND  (TQD.FLAG_DEBET = 1)
              UNION ALL  -- KREDIT
              SELECT  ((-1) * TQK.QUANT) AS QUANT
              FROM    TABL$P_TMC_QUANT_MOVE TQK
              WHERE  (TQK.TMC_ID = :P_TMC_ID)
                AND  (TQK.DATE_COMMIT BETWEEN :DATE_RPT AND DATEADD(1 DAY TO :DATE_RPT))
                AND  (:FIRM_IDS  CONTAINING '~'||TQK.FIRM_ID ||'~')
                AND  (:P_PLACE_IDS CONTAINING '~'||TQK.PLACE_ID||'~')
                AND  (TQK.FLAG_DEBET = 0)
              )Q
      INTO  :P_QUANT;
      S0011 = :S0011 + (:P_QUANT * :P_PRICE_IN);        
      END          
    S0011 = :S0011 + :S0002;

    FOR
      SELECT TQK.TMC_ID, TQK.PRICE_TMC, TQK.QUANT
      FROM   TABL$P_TMC_QUANT_MOVE TQK
      WHERE  (TQK.DATE_COMMIT BETWEEN :DATE_RPT AND DATEADD(1 DAY TO :DATE_RPT))
        AND  (:FIRM_IDS  CONTAINING '~'||TQK.FIRM_ID ||'~')
        AND  (:P_PLACE_IDS CONTAINING '~'||TQK.PLACE_ID||'~')
        AND  (TQK.FLAG_DEBET = 0)
      INTO   :P_TMC_ID, :P_PRICE, :P_QUANT  
    DO
      BEGIN
      SELECT FIRST 1 COALESCE(D.PRICE,0)
      FROM   TABL$D_1000014 D, TABL$J_4 J
      WHERE  (D.TMC_ID = :P_TMC_ID)
        AND  (J.ID = D.J_ID)
        AND  (J.TYPE_ID = 1000026)
        AND  (J.FLAG_COMMIT = 1)
      INTO   :P_PRICE_IN;
      S0003 = :S0003 + (:P_PRICE    * :P_QUANT);
      S0006 = :S0006 + (:P_PRICE_IN * :P_QUANT);
      END  

    SELECT COALESCE(SUM(J.DOCSUM),0)
    FROM   TABL$J_4 J
    WHERE  (J.DATE_COMMIT BETWEEN :DATE_RPT AND DATEADD(1 DAY TO :DATE_RPT))
      AND  (J.TYPE_ID = 1000026)
      AND  (:FIRM_IDS CONTAINING '~'||J.FIRM_ID||'~')
      AND  (J.FLAG_COMMIT = 1)
    INTO   :S0005;

    SELECT COALESCE(SUM(J.DOCSUM),0)
    FROM   TABL$J_4 J, TABL$J_1000016 J16
    WHERE  (J.DATE_COMMIT BETWEEN :DATE_RPT AND DATEADD(1 DAY TO :DATE_RPT))
      AND  (J.TYPE_ID = 1000021)
      AND  (:FIRM_IDS CONTAINING '~'||J.FIRM_ID||'~')
      AND  (J.FLAG_COMMIT = 1)
      AND  (J16.J_ID = J.ID)
      AND  ('~661~6611~6612~' CONTAINING '~'||J16.ACC_ID||'~')
    INTO   :S0009;

    SELECT COALESCE(SUM(J.DOCSUM),0)
    FROM   TABL$J_4 J, TABL$J_1000016 J16
    WHERE  (J.DATE_COMMIT BETWEEN :DATE_RPT AND DATEADD(1 DAY TO :DATE_RPT))
      AND  (J.TYPE_ID = 1000021)
      AND  (:FIRM_IDS CONTAINING '~'||J.FIRM_ID||'~')
      AND  (J.FLAG_COMMIT = 1)
      AND  (J16.J_ID = J.ID)
      AND  ('~301~311~661~6611~6612~651~631~' NOT CONTAINING '~'||J16.ACC_ID||'~')
    INTO   :S0010;

    S0012 = :S0003 - :S0006 - :S0009 - :S0010;
    SUSPEND;
    DATE_RPT = DATEADD(1 DAY TO :DATE_RPT);
    END
END
