# SmarTree
- 아두이노 Slave
- 트리 조명 조절, LCD에 응원 메세지 출력
---
- SmarTree.ino : Main, Master로부터 데이터 받아서 데이터 Parsing, 메세지 LCD 출력, Master 요청이 있는 경우 조도센서 값 측정해 Master에 전달
- controlNeopixels.ino : 할일 완료 여부/지정 색상에 따른 LED 색상 조절
- ringCarol.ino : 수동부저로 캐롤 재생
