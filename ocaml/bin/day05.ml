let parse_rule str = Scanf.sscanf str "%d|%d" (fun a b -> (a, b))

let parse_update str =
  List.map (fun a -> int_of_string a) (String.split_on_char ',' str)

let read_rules filename =
  let file = open_in filename in
  let lines = In_channel.input_lines file in
  close_in file;

  let rec process_lines lines acc =
    match lines with
    | [] -> acc
    | h :: _ when String.length h == 0 -> acc
    | h :: t -> process_lines t (acc @ [ parse_rule h ])
  in

  process_lines lines []

let read_updates filename =
  let file = open_in filename in
  let lines = In_channel.input_lines file in
  close_in file;

  let rec process_lines lines started acc =
    match (lines, started) with
    | h :: t, false when String.length h == 0 -> process_lines t true []
    | _ :: t, false -> process_lines t false []
    | h :: t, true -> process_lines t true (acc @ [ parse_update h ])
    | [], _ -> acc
  in

  process_lines lines false []

let is_update_valid rules update =
  let is_invalid invalidator to_check rules =
    List.exists (fun a -> fst a == to_check && snd a == invalidator) rules
  in

  let rec valid_before l rules =
    match l with
    | h :: t when List.exists (fun to_check -> is_invalid h to_check rules) t ->
        false
    | _ :: t -> valid_before t rules
    | [] -> true
  in

  valid_before update rules

let middle_elem l = List.nth l (List.length l / 2)

let silver () =
  let rules = read_rules "../inputs/05.txt" in
  let updates = read_updates "../inputs/05.txt" in

  let valid = List.filter (is_update_valid rules) updates in
  List.fold_left (fun acc update -> acc + middle_elem update) 0 valid
;;

print_int (silver ())
