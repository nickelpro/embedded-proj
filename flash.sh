port=""

while [ -z $port ]
do
  port=`find /dev -name 'ttyACM*'`
done

echo "Found $port"
avrdude -p m32u4 -c avr109 -U flash:w:build/Release/flash:e -P $port
