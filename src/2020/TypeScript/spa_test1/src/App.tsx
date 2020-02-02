import React, { useCallback, useEffect } from "react";
import "./styles.css";

const sock = new WebSocket('ws://127.0.0.1:8080');
sock.addEventListener('open', e => {
  console.log('ws opened');
});
sock.addEventListener('message', e => {
  console.log(e.data);
});

export const App = () => {
  useEffect(() => {
  }, []);
  const onClick = useCallback(() => {
    sock.send('test!');
  }, []);

  return (
    <div className="App">
      <button onClick={onClick}>send</button>
    </div>
  );
}
