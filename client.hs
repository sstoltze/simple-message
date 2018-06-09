-- From https://wiki.haskell.org/Implement_a_chat_server

module Main where

import Network.Socket
import System.IO

port :: PortNumber
port = 8080

mainServer :: IO ()
mainServer = do
  sock <- socket AF_INET Stream 0
  setSocketOption sock ReuseAddr 1
  bind sock (SockAddrInet port $ tupleToHostAddress (127, 0, 0, 1))
  listen sock 5
  mainLoop sock

main :: IO ()
main = do
  sock <- socket AF_INET Stream 0
  -- setSocketOption sock ReuseAddr 1
  let addr = SockAddrInet port $ tupleToHostAddress (127, 0, 0, 1)
  connect sock addr
  runConn (sock, addr)

mainLoop :: Socket -> IO ()
mainLoop sock = do
  putStrLn "mainLoop"
  connection <- accept sock
  runConn connection
  mainLoop sock

runConn :: (Socket, SockAddr) -> IO ()
runConn (sock, _) = do
  putStrLn "runConn"
  handle <- socketToHandle sock ReadWriteMode
--  hSetBuffering handle NoBuffering
  hPutStrLn handle "Hello from Haskell!\n"
  hClose handle -- Also closes socket
