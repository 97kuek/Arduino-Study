# 光通信モールス信号受信デバイス 設計書

## 1. 概要
光（懐中電灯やLEDなど）の点滅によるモールス信号を受光素子で受け取り、解読した文字を有機ELキャラクタデバイスに表示するデバイスを作成する。

## 2. システム構成
- **入力**: 光信号 (短点・長点)
- **処理**: Arduinoによる電圧計測、時間計測、モールス符号の復号
- **出力**: I2C接続のOLEDディスプレイへの文字表示

## 3. ハードウェア設計

### 3.1 必要な部品
1.  **Arduinoボード** (Uno R3)
2.  **受光素子**: 
    - フォトトランジスタ (例: NJL7502L) または CdSセル
    - 抵抗 (10kΩ程度)
3.  **表示デバイス**:
    - **LCD1602 (16ピン)**
    - 抵抗 (コントラスト調整用: 1kΩ〜10kΩ程度、または可変抵抗があると望ましい)
4.  **その他**:
    - ジャンパワイヤ、ブレッドボード

### 3.2 回路設計

![回路図](回路図.png)

## 4. ソフトウェア設計


### 4.1 機能要件
1.  **閾値判定**: アナログ入力値がある閾値を超えたら「光あり(ON)」、下回ったら「光なし(OFF)」と判定する。
2.  **時間計測**: ONの状態が続いた時間、OFFの状態が続いた時間をミリ秒単位で計測する。
3.  **信号判定**:
    - **短点 (Dot)**: ON時間が短い (例: < 200ms)
    - **長点 (Dash)**: ON時間が長い (例: > 200ms)
    - **文字区切り**: OFF時間が一定以上 (例: > 400ms) で1文字確定
    - **単語区切り**: OFF時間がさらに長い (例: > 1000ms) でスペース挿入
4.  **デコード**: 記録した短点・長点の組み合わせを「A-Z, 0-9」等の文字に変換する。
5.  **表示**: 確定した文字をOLEDに順次表示する。画面がいっぱいになったらスクロール等の処理を行う。

### 4.2 処理フロー (フローチャート)

```mermaid
flowchart TD
    Start([開始]) --> Setup[セットアップ: ピン設定, 変数初期化]
    Setup --> Loop_Start{ループ開始}

    Loop_Start --> Read_Sensor[光センサー値の読み取り]
    Read_Sensor --> Check_Threshold{閾値判定}

    %% 光あり (ON) の場合
    Check_Threshold -- "閾値以上 (ON)" --> Is_State_Changed_On{状態変化?}
    
    Is_State_Changed_On -- "OFFからONへ変化" --> Measure_Off_Time[OFF時間の計測終了]
    Measure_Off_Time --> Reset_Timer_On[ON開始時刻を記録]
    Reset_Timer_On --> Set_State_On[状態をONに更新]
    
    Is_State_Changed_On -- "ON継続中" --> Loop_Start

    %% 光なし (OFF) の場合
    Check_Threshold -- "閾値未満 (OFF)" --> Is_State_Changed_Off{状態変化?}
    
    Is_State_Changed_Off -- "ONからOFFへ変化" --> Measure_On_Time[ON時間の計測終了]
    Measure_On_Time --> Identify_Signal{長さ判定}
    Identify_Signal -- "短い (短点)" --> Add_Dot[バッファに'.'追加]
    Identify_Signal -- "長い (長点)" --> Add_Dash[バッファに'-'追加]
    
    Add_Dot --> Reset_Timer_Off[OFF開始時刻を記録]
    Add_Dash --> Reset_Timer_Off
    Reset_Timer_Off --> Set_State_Off[状態をOFFに更新]
    
    Is_State_Changed_Off -- "OFF継続中" --> Check_Timeout{経過時間確認}
    
    Check_Timeout -- "文字区切り時間超過 & 未処理" --> Decode_Char[バッファを文字に変換して表示]
    Decode_Char --> Clear_Buffer[バッファクリア]
    Clear_Buffer --> Loop_Start
    
    Check_Timeout -- "単語区切り時間超過 & 未処理" --> Insert_Space[スペースを表示]
    Insert_Space --> Loop_Start
    
    Check_Timeout -- "区切り未達" --> Loop_Start
    
    Set_State_On --> Loop_Start
    Set_State_Off --> Loop_Start
```
