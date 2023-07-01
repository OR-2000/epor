tpr_limits=(0)

gcs=(1 2)

expcnts=1
backoffs=(0)
sleeplongs=(1000)

recs=(10000000) # 1024, 16384, 131072 1048576, 16777216
skews=(0.8)
reqs=(10)
reqlongs=(0 50 100 500 1000 5000 10000)
threads=(64)
actives=(100)

EXEC_SEC=100
OPT=0
# params below is useless when OPT is 0.
OPT_INTERVAL_MS=5
OPT_DELETION_THRESHOLD=20
OPT_MAX_COUNT=3

PARAMS_COMMON=-DEXEC_SEC=$EXEC_SEC\
\ -DOPT=$OPT\
\ -DOPT_INTERVAL_MS=$OPT_INTERVAL_MS\
\ -DOPT_DELETION_THRESHOLD=$OPT_DELETION_THRESHOLD\
\ -DOPT_MAX_COUNT=$OPT_MAX_COUNT\

READ_RATIO=40 # 次100 shortのwriteが90
READ_RATIO_LONG=100

# long: 100, short: 10 => 1.1倍の性能差
# 次の仮説：　longにwriteが入るとおかしくなる
# スキューを0.99まで上げてみる => 1.5倍の性能差
# 一部のレコードのチェーンだけが伸びていて、時々くるリードは古いやつ読み込むから通る
# hignコンテンションだとepo, no contentionだとepo-rの方がうまく行く
# skew 0だと、epo-rの方がうまく行く

# version chainが長ければread/write時関係なくGCをかける最適化


GFLAGS_COMMON="--read_ratio $READ_RATIO
--read_ratio_long $READ_RATIO_LONG"

