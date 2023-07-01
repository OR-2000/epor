#!/bin/bash

source ./param.bash

# rm -rf log
mkdir -p log log/info

for gc in ${gcs[@]}
do
  for backoff in ${backoffs[@]}
  do
    for sleeplong in ${sleeplongs[@]}
    do
      PARAMS=$PARAMS_COMMON\
\ -DSLEEP_US_LONG=$sleeplong\
\ -DBACKOFF_US=$backoff\
\ -DGC_MODE=$gc
      
      rm -rf ./src/build
      mkdir ./src/build
      cd ./src/build

      cmake .. $PARAMS
      make
      cd ../../

      for rec in ${recs[@]}
      do
        for skew in ${skews[@]}
        do
          for req in ${reqs[@]}
          do
            for reqlong in ${reqlongs[@]}
            do
              for active in ${actives[@]}
              do
                for tpr_limit in ${tpr_limits[@]}
                do
                  for i in `seq $expcnts`
                  do
                    FILE_NAME=`date +%Y%m%d%H%M%S`
                    for thread in ${threads[@]}
                    do
                      GFLAGS="$GFLAGS_COMMON
    --thread_size $thread
    --filename $FILE_NAME
    --record_size $rec
    --skew $skew
    --request_size $req
    --request_size_long $reqlong
    --actives_update_interval_ms $active
    --tpr_limit $tpr_limit"
                      # numactl --interleave=all ./src/build/si.exe $GFLAGS
                      ./src/build/si.exe $GFLAGS
                    done
                  done
                done
              done
            done
          done
        done
      done
    done
  done
done

# scp -r log onishi2000@157.185.197.11:~/
# rm -rf log