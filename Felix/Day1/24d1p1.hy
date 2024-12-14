(print(sum

    ; take difference of each row
    (lfor rowOfSorted

      ; transpose  
      (zip (unpack-iterable

        ; sort each column of the file
        (lfor  col

          ; transpose 
          (zip (unpack-iterable

            ; Read the file into lines
            (lfor line
              (.readlines (open "Real.txt" "r"))

              ; Split and strip the line, and parse each string to int
              (lfor num (.split (.strip line) "   ") (int num) )
            )
                
          ))
          (sorted (list col))
        )
            
      ))
      
      (abs (- (unpack-iterable rowOfSorted)))
    )
))
