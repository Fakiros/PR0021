if [ -f counter.txt ] ;
then
  echo $((`cat counter.txt`+1)) > counter.txt ;
else
  echo "1"  > counter.txt ;
fi
echo "unsigned int version PROGMEM = `cat counter.txt`;" > version.h ;
  
