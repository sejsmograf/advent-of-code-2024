let read_input filename =
  let in_channel = open_in filename in
  let lines = In_channel.input_lines in_channel in
  close_in in_channel;

  let parse_line (line : string) : int list =
    let numbers = String.split_on_char ' ' line in
    List.map (fun s -> int_of_string s) numbers
  in

  List.map (fun line -> parse_line line) lines

let is_report_safe (report : int list) : bool =
  let rec iterate l increasing =
    match l with
    | [] -> true
    | _ :: [] -> true
    | h1 :: h2 :: t -> (
        let diff = h1 - h2 in
        match (diff, increasing) with
        | v, _ when abs v > 3 || abs v == 0 -> false
        | _, None -> iterate (h2 :: t) (Some (h1 < h2))
        | v, Some true -> if v > 0 then false else iterate (h2 :: t) (Some true)
        | v, Some false ->
            if v < 0 then false else iterate (h2 :: t) (Some false))
  in

  iterate report None

let _silver () =
  let input = read_input "../inputs/02.txt" in

  let res =
    List.fold_left
      (fun acc report -> acc + if is_report_safe report then 1 else 0)
      0 input
  in

  print_int res

let gold () =
  let input = read_input "../inputs/02.txt" in

  let rec check_all_removals (prefix : int list) (elem : int)
      (suffix : int list) : bool =
    match suffix with
    | _h :: _t when is_report_safe (prefix @ suffix) -> true
    | h :: t -> check_all_removals (prefix @ [ elem ]) h t
    | [] when is_report_safe (prefix @ suffix) -> true
    | [] -> false
  in

  let rec summarize input sum =
    match input with
    | [] -> sum
    | first :: rest -> (
        match first with
        | l when is_report_safe l -> summarize rest (sum + 1)
        | h :: t when check_all_removals [] h t -> summarize rest (sum + 1)
        | _ -> summarize rest sum)
  in

  let res = summarize input 0 in
  print_int res

let () = gold ()
