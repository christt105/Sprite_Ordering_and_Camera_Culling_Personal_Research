<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.2.2" name="Player" tilewidth="14" tileheight="21" tilecount="9" columns="3">
 <properties>
  <property name="AnimationSpeed" type="float" value="5"/>
 </properties>
 <image source="Player.png" width="42" height="63"/>
 <tile id="0">
  <animation>
   <frame tileid="0" duration="200"/>
  </animation>
 </tile>
 <tile id="3">
  <animation>
   <frame tileid="0" duration="200"/>
   <frame tileid="3" duration="200"/>
   <frame tileid="0" duration="200"/>
   <frame tileid="6" duration="200"/>
  </animation>
 </tile>
</tileset>
