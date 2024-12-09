let read_input filename =
  let file = open_in filename in
  let lines = In_channel.input_lines file in
  close_in file;

  let get_target line = Scanf.sscanf line "%d: " (fun d -> d) in
  let get_numbers line =
    List.map
      (fun a -> int_of_string a)
      (String.split_on_char ' '
         (String.sub line
            (String.index line ':' + 2)
            (String.length line - String.index line ':' - 2)))
  in
  let process_line line = (get_target line, get_numbers line) in

  let rec process_lines lines acc =
    match lines with
    | [] -> acc
    | h :: t -> process_lines t (process_line h :: acc)
  in

  process_lines lines []

let is_line_solveable line gold =
  let rec solve target numbers current_sum =
    match (target, numbers) with
    | n, [] -> n == current_sum
    | _, h :: t ->
        if gold then
          solve target t (current_sum + h)
          || solve target t (current_sum * h)
          || solve target t
               (int_of_string (string_of_int current_sum ^ string_of_int h))
        else solve target t (current_sum + h) || solve target t (current_sum * h)
  in
  match snd line with [] -> false | h :: t -> solve (fst line) t h

let silver () =
  let input = read_input "../inputs/07.txt" in

  let valid = List.filter (fun a -> is_line_solveable a false) input in
  List.fold_left (fun acc v -> acc + fst v) 0 valid

let gold () =
  let input = read_input "../inputs/07.txt" in

  let valid = List.filter (fun a -> is_line_solveable a true) input in
  List.fold_left (fun acc v -> acc + fst v) 0 valid
;;

print_int (silver ());
print_char '\n';
print_int (gold ())
