fn="zoepants_72_93.png"
hs=`echo ${fn} | sed 's/^.*_\([0-9]*\)_\([0-9]*\)\.png$/\1/'`
vs=`echo ${fn} | sed 's/^.*_\([0-9]*\)_\([0-9]*\)\.png$/\2/'`
echo "my hs is ${hs}"
