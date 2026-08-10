# Alt/GUI key morph behavior

`modula,behavior-alt-gui-morph` は通常時、Alt 押下時、GUI 押下時で別々の binding を送る behavior です。レイヤーは変更しません。ZMK v0.3 の behavior 引数は2個までなので、組み合わせごとに node を定義します。

```dts
agm_vol_up: alt-gui-volume-up {
    compatible = "modula,behavior-alt-gui-morph";
    #binding-cells = <0>;
    bindings = <&kp C_VOL_UP>, <&kp LS(TAB)>, <&kp RIGHT>;
};
```

この例では、通常時は Volume Up、Alt と同時なら Shift+Tab、GUI と同時なら Right Arrow を送ります。Alt と GUI を同時に押している場合は GUI 側を優先します。`bindings` には通常のキーコード、Consumer キーコード、`LC(TAB)` のような修飾付きキーコードに加え、他の behavior も指定できます。

現在は `config/Modula.keymap` の左下から2個目のキーを `&agm_mute` に置き換えています。encoder は通常時に Volume Up/Down、Alt 時に Shift+Tab/Tab、GUI 時に Right/Left Arrow を送ります。
