cmd="/Users/chraibi/Workspace/jupedsim/Amristart/sim/bin/jpscore"
for f in `ls ini*.xml`; do
    echo $f
    $cmd $f
done
