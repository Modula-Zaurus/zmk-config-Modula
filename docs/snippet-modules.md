# Modula のモジュール snippets

現段階では右半分を central、左半分を peripheral とする既存の役割分担を維持します。モジュールは snippet を組み合わせて選択します。

- トラックボール: `trackball_pat9125`、`trackball_paw3222`、`pmw3610_alt`
- EC11 スロット 1: `encoder_ec11_left`（P1.13 / P1.14）
- EC11 スロット 2: `encoder_ec11_right`（P1.12 / P1.15）

`trackball_pat9125` は、モジュール上の EC11（P1.12 / P1.15）も同時に有効化します。別スロットの `encoder_ec11_left`（P1.13 / P1.14）とは GPIO が異なるため、左 peripheral では次のように同時指定できます。

```yaml
snippet: trackball_pat9125 encoder_ec11_left
```

`encoder_ec11_right` は PAT9125 モジュール内 EC11 と同じ P1.12 / P1.15 を使うため、`trackball_pat9125` と同時指定しません。また、P1.12 を chip-select に使う SPI トラックボールとも併用しません。
