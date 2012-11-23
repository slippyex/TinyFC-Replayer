#include "tiny_fc.bi"
dim shared dummy as integer
initFC(0, @dummy, 0, "sample.fc")
startFC()
    print "Press any key to exit..."
sleep
stopFC()