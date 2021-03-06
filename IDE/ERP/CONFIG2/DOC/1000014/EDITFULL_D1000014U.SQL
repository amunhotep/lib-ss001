EXECUTE BLOCK (
   Q_D_ID                   TYPE OF COLUMN TABL$D_1000014.ID           = ?ID
  ,Q_D_NAME                 TYPE OF COLUMN TABL$D_1000014.NAME         = ?NAME
  ,Q_TMC_ID                 TYPE OF COLUMN TABL$D_1000014.TMC_ID       = ?TMC_ID
  ,Q_PRICE_TMC              TYPE OF COLUMN TABL$D_1000014.PRICE_TMC    = ?PRICE_TMC
  ,Q_PRICE                  TYPE OF COLUMN TABL$D_1000014.PRICE        = ?PRICE
  ,Q_QUANTORDER             TYPE OF COLUMN TABL$D_1000014.QUANT        = ?QUANTORDER
  ,Q_QUANT                  TYPE OF COLUMN TABL$D_1000014.QUANT        = ?QUANT
  ,Q_TOTALAMOUNT            TYPE OF COLUMN TABL$D_1000014.TOTALAMOUNT  = ?TOTALAMOUNT
  ,Q_ENTRYDATE              TYPE OF COLUMN TABL$D_1000014.ENTRYDATE    = ?ENTRYDATE
  ,Q_ENTRYBILL              TYPE OF COLUMN TABL$D_1000014.ENTRYBILL    = ?ENTRYBILL
)AS
  DECLARE VARIABLE P_QUANTORDER TYPE OF COLUMN TABL$D_1000014.QUANT;
  DECLARE VARIABLE P_QUANT      TYPE OF COLUMN TABL$D_1000014.QUANT;
BEGIN
  UPDATE TABL$D_1000014 DB SET
    DB.TMC_ID      = :Q_TMC_ID
   ,DB.PRICE_TMC   = :Q_PRICE_TMC
   ,DB.QUANT       = :Q_QUANT
   ,DB.PRICE       = :Q_PRICE
   ,DB.TOTALAMOUNT = :Q_TOTALAMOUNT
   ,DB.ENTRYDATE   = :Q_ENTRYDATE
   ,DB.ENTRYBILL   = :Q_ENTRYBILL
   ,DB.NAME        = :Q_D_NAME
  WHERE  (DB.ID = :Q_D_ID);
END
