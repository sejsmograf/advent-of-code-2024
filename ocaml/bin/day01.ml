let read_lines (filename : string) : string list =
  let in_channel = open_in filename in
  let lines = In_channel.input_lines in_channel in
  close_in in_channel;
  lines

let parse_input (lines : string list) : int list * int list =
  let extract_integers str = Scanf.sscanf str "%d %d" (fun a b -> (a, b)) in

  let rec create_reports (lines : string list) (acc : int list * int list) =
    match lines with
    | [] -> acc
    | head :: tail ->
        let parsed = extract_integers head in
        let new_fst = fst acc @ [ fst parsed ] in
        let new_snd = snd acc @ [ snd parsed ] in
        create_reports tail (new_fst, new_snd)
  in

  create_reports lines ([], [])

let silver =
  let filename = "../inputs/01.txt" in
  let lines = read_lines filename in
  let parsed = parse_input lines in
  let sorted_fst = List.sort (fun a b -> a - b) (fst parsed) in
  let sorted_snd = List.sort (fun a b -> a - b) (snd parsed) in

  let rec iterate one two total_distance =
    match (one, two) with
    | [], [] -> total_distance
    | h1 :: t1, h2 :: t2 -> iterate t1 t2 (abs (h1 - h2) + total_distance)
    | _, _ -> total_distance
  in

  iterate sorted_fst sorted_snd 0

let gold =
  let filename = "../inputs/01.txt" in
  let lines = read_lines filename in
  let parsed = parse_input lines in

  let create_counts one two =
    let counts = Hashtbl.create (List.length one) in

    let rec count_elements l =
      match l with
      | [] -> ()
      | h :: t ->
          let elem_count =
            List.fold_left (fun acc x -> acc + if x == h then 1 else 0) 0 two
          in
          Hashtbl.add counts h elem_count;
          count_elements t
    in

    count_elements one;
    counts
  in

  let counts = create_counts (fst parsed) (snd parsed) in

  let rec iterate one sum =
    match one with
    | [] -> sum
    | h :: t -> iterate t (sum + (h * Hashtbl.find counts h))
  in

  iterate (fst parsed) 0

let () =
  let _silver = silver in
  let result = gold in
  print_int result
