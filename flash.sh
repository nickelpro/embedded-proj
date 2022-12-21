port=""
while [ -z $port ]
do
  port=`find /dev -name 'ttyACM*'`
done

echo "Running: avrdude -p m32u4 -c avr109 -U flash:w:build/release/flash:e $port"
avrdude -p m32u4 -c avr109 -U flash:w:build/release/flash:e $port

cat $port
