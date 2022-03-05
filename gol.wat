(module
  (import "console" "log" (func $log (param i32)))
  (import "js" "mem" (memory 1))
  (func (export "nextStep") (param $width i32) (param $height i32)
    (local $step i32)
    (local $offset i32)
    (local $nextOffset i32)
    (local $x i32)
    (local $y i32)
    (local $dx i32)
    (local $dy i32)
    (local $neighbors i32)
    (local $temp1 i32)
    (local $temp2 i32)
    (local $temp3 i32)

    ;; let offset = memory[0] === 0 ? 1 : width * height + 1;
    (local.set $step (i32.load (i32.const 0)))
    (if (i32.eqz (local.get $step))
      (then
        (local.set $offset (i32.const 1))
      )
      (else
        (local.set $offset (i32.add (i32.mul (local.get $width) (local.get $height)) (i32.const 1)))
      )
    )

    ;; let nextOffset = memory[0] === 1 ? 1 : width * height + 1;
    (if (i32.eq (local.get $step) (i32.const 1))
      (then
        (local.set $nextOffset (i32.const 1))
      )
      (else
        (local.set $nextOffset (i32.add (i32.mul (local.get $width) (local.get $height)) (i32.const 1)))
      )
    )

    ;; for (let y = 1; y < height - 1; y++) {
    (local.set $y (i32.const 1))
    (block $yblock (loop $yloop
      (br_if $yblock (i32.ge_u (local.get $y) (i32.sub (local.get $height) (i32.const 1))))

      ;; for (let x = 1; x < width - 1; x++) {
      (local.set $x (i32.const 1))
      (block $xblock (loop $xloop
        (br_if $xblock (i32.ge_u (local.get $x) (i32.sub (local.get $width) (i32.const 1))))

        ;; let neighbors = 0;
        (local.set $neighbors (i32.const 0))

        ;; for (let dy = 0; dy <= 2; dy++) {
        (local.set $dy (i32.const 0))
        (block $dyblock (loop $dyloop
          (br_if $dyblock (i32.gt_u (local.get $dy) (i32.const 2)))

          ;; for (let dx = 0; dx <= 2; dx++) {
          (local.set $dx (i32.const 0))
          (block $dxblock (loop $dxloop
            (br_if $dxblock (i32.gt_u (local.get $dx) (i32.const 2)))

            ;; if (memory[offset + x + dx - 1 + (y + dy - 1) * width] === 1) {
            (local.set $temp1 (i32.sub (i32.add (local.get $x) (local.get $dx)) (i32.const 1))) ;; x + dx - 1
            (local.set $temp2 (i32.sub (i32.add (local.get $y) (local.get $dy)) (i32.const 1))) ;; y + dy - 1
            (local.set $temp3 (i32.add (i32.add (local.get $offset) (local.get $temp1)) (i32.mul (local.get $temp2) (local.get $width))))
            (if (i32.eq (i32.load (i32.mul (local.get $temp3) (i32.const 4))) (i32.const 1)) (then
              ;; neighbors += 1;
              (local.set $neighbors (i32.add (local.get $neighbors) (i32.const 1)))
            ))

            (local.set $dx (i32.add (local.get $dx) (i32.const 1)))
            (br $dxloop)
          ))
          (local.set $dy (i32.add (local.get $dy) (i32.const 1)))
          (br $dyloop)
        ))

        ;; if (memory[offset + x + y * width] === 0) {
        (local.set $temp1 (i32.add (i32.add (local.get $offset) (local.get $x)) (i32.mul (local.get $y) (local.get $width))))
        (if (i32.eqz (i32.load (i32.mul (local.get $temp1) (i32.const 4))))
          (then
            ;; if (neighbors === 3) {
            (if (i32.eq (local.get $neighbors) (i32.const 3))
              (then
                ;; memory[nextOffset + x + y * width] = 1;
                (local.set $temp1 (i32.add (i32.add (local.get $nextOffset) (local.get $x)) (i32.mul (local.get $y) (local.get $width))))
                (i32.store (i32.mul (local.get $temp1) (i32.const 4)) (i32.const 1))
              )
              (else
                ;; memory[nextOffset + x + y * width] = 0;
                (local.set $temp1 (i32.add (i32.add (local.get $nextOffset) (local.get $x)) (i32.mul (local.get $y) (local.get $width))))
                (i32.store (i32.mul (local.get $temp1) (i32.const 4)) (i32.const 0))
              )
            )
          )
          (else
            ;; if (neighbors <= 2 || 5 <= neighbors) {
            (if (i32.le_u (local.get $neighbors) (i32.const 2))
              (then
                ;; memory[nextOffset + x + y * width] = 0;
                (local.set $temp1 (i32.add (i32.add (local.get $nextOffset) (local.get $x)) (i32.mul (local.get $y) (local.get $width))))
                (i32.store (i32.mul (local.get $temp1) (i32.const 4)) (i32.const 0))
              )
              (else
                (if (i32.le_u (i32.const 5) (local.get $neighbors))
                  (then
                    ;; memory[nextOffset + x + y * width] = 0;
                    (local.set $temp1 (i32.add (i32.add (local.get $nextOffset) (local.get $x)) (i32.mul (local.get $y) (local.get $width))))
                    (i32.store (i32.mul (local.get $temp1) (i32.const 4)) (i32.const 0))
                  )
                  (else
                    ;; memory[nextOffset + x + y * width] = 1;
                    (local.set $temp1 (i32.add (i32.add (local.get $nextOffset) (local.get $x)) (i32.mul (local.get $y) (local.get $width))))
                    (i32.store (i32.mul (local.get $temp1) (i32.const 4)) (i32.const 1))
                  )
                )
              )
            )
          )
        )

        (local.set $x (i32.add (local.get $x) (i32.const 1)))
        (br $xloop)
      ))
      (local.set $y (i32.add (local.get $y) (i32.const 1)))
      (br $yloop)
    ))

    ;; memory[0] = memory[0] === 0 ? 1 : 0;
    (if (i32.eqz (i32.load (i32.const 0)))
      (then
        (i32.store (i32.const 0) (i32.const 1))
      )
      (else
        (i32.store (i32.const 0) (i32.const 0))
      )
    )
  )
)
