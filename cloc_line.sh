
echo "######## arch ########"
cloc arch --fullpath

echo "######## drivers ########"
cloc drivers --fullpath

echo "######## include ########"
cloc include --fullpath

echo "######## init ########"
cloc init --fullpath

echo "######## kernel ########"
cloc kernel --fullpath

echo "######## libs ########"
cloc lib --fullpath

echo "######## mm ########"
cloc mm --fullpath

echo "######## sum ########"
dirs="arch drivers include init kernel lib mm"
cloc ${dirs} --fullpath
