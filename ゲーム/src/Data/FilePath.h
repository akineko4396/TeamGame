#pragma once

//プレイヤーパス
#define PLAYER_PATH "data/original/Player/Player3.xed"

//マップパス
#define MAP_PATH "data/model/map2/map.x"


//テキスト簡略化
#define TXT_DATA_SAVE "\t" << "Pos(" << Pos.x << "," << Pos.y << "," << Pos.z << ")" << "\t" << "Scale(" << Scale.x << "," << Scale.y << "," << Scale.z << ")" << "\t" << "Angle(" << Angle.x << "," << Angle.y << "," << Angle.z << ")" 

//CSV簡略化
#define CSV_DATA_SAVE Pos.x << "," << Pos.y << "," << Pos.z << "," << size << "," << Angle.x << "," << Angle.y << "," << Angle.z << ","