#! /bin/sh
echo Let\'s run some tests!
make
./writetests
testFiles=$(grep -r -i -l --include=\*.um '' ./)
#echo $testFiles

for testFile in $testFiles ; do
    testName=$(echo $testFile | sed -E 's/(.*).um/\1/')
    outputFile=$(echo $testName.1)
    if [ -f $outputFile ] ; then
        inputFile=$(echo $testName.0)
        if [ -f $inputFile ] ; then
            ./um $testName.um < $testName.0 > $testName.out
            diff --text $testName.out $testName.1
            echo "$testName passed diff!"
        else
            ./um $testName.um > $testName.out
            diff --text $testName.out $testName.1
            echo "$testName passed diff!"
        fi
    else
        ./um $testName.um
        echo "$testName has no output!"
    fi
done

