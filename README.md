# BilliardsGame

![2016-12-15 03 09 09](https://cloud.githubusercontent.com/assets/9264352/21194755/677a9262-c274-11e6-80de-4f556d837f92.png)

### 現状できているもの ###
* FBXの読み込み
* 複数モデルの描画
* モデルの移動、回転、スケール
* Phongシェーダーで描画（正しく描画できていない可能性あり）
* サウンドの再生
* インプット処理
* PhysXの初期化
* PhysXでの物理移動
* オブジェクト同士の衝突判定
* ゲーム部分

===============================
### TODO ###
===============================
* Phongシェーダーの修正（phongの反射の位置がおかしい）
* 複数ライトに対応
* 影の実装
* 一部ゲーム部分

===============================
### 使用方法 ###
===============================
* 「実行データ」のフォルダー内にデータが入っている

* AwoEngine.exeをクリックすると画面が開く

* しばらく待つと読み込みが完了する

* ゲームルールは8ボール + 少しルールをかみ砕いた

* 左側のテキストには上から
* コマンドリスト （コマンドのリストが表示）
* カレントターン　（現在どちらのターンかを表示）
* ボールステート　（それぞれのボールがポケットに落ちているかを表示）

* ゲーム設定ウィンドウ
* カメラセッティングウィンドウ　（カメラの位置や注視点を指定できる）
* シューターセッティング　（ボールのショットに関する設定を行える）

* シューターセッティング詳細
* CanShoot　（ボールを打てるかどうか、すべてのボールが停止するとシュートできるようになる） 確認用
* CanFreeMove　（次に打つボールが自由に移動可能かを確認できる）　確認用 FreeMoveは未実装
* CanKitchenMove　（次に打つボールがキッチン内で移動可能かを確認できる）　確認用　KitchenMoveは未実装
* ArrowQuat　（球の発射先を示す矢印のクォータニオンを確認できる） 確認用
* ShootingAngle　（球の発射角度を設定できる）
* ShootingPower　（球の発射力を設定できる）
* ArrowScale （球の発射先を示す矢印の大きさを設定できる）
