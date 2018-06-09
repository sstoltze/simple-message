(* Based on https://rosettacode.org/wiki/Sockets#OCaml *)
open Unix

(* let PORT = 8080 *)

let init_socket addr port =
  let inet_addr = (gethostbyname addr).h_addr_list.(0) in
  let sockaddr = ADDR_INET (inet_addr, port) and
      sock     = socket PF_INET SOCK_STREAM 0 in
  connect sock sockaddr;
  let outchan = out_channel_of_descr sock and
      inchan  = in_channel_of_descr sock in
  (inchan, outchan);;

let () =
  let ic, oc = init_socket "localhost" 8080 in
  output_string oc "Hello from ocaml";
  close_out oc;
  try
    let line = input_line ic in
    print_endline line;
    close_in ic;
  with e -> close_in_noerr ic;
            raise e;;
