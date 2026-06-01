import { useState, useEffect } from 'react';
import { io } from 'socket.io-client';
import './App.css';

const sk = io('http://localhost:3000');

function App() {
  const [b, sb] = useState(Array(9).fill(0));
  const [w, sw] = useState(null);
  const [wl, swl] = useState([]);

  const cw = (bd) => {
    const ls = [[0,1,2],[3,4,5],[6,7,8],[0,3,6],[1,4,7],[2,5,8],[0,4,8],[2,4,6]];
    for (let i = 0; i < ls.length; i++) {
      const [x, y, z] = ls[i];
      if (bd[x] && bd[x] === bd[y] && bd[x] === bd[z]) {
        return { wn: bd[x], ln: ls[i] };
      }
    }
    if (!bd.includes(0)) return { wn: 3, ln: [] };
    return null;
  };

  useEffect(() => {
    sk.on('res', (m) => {
      const am = parseInt(m, 10);
      if (am >= 0 && am <= 8) {
        sb(p => {
          const nb = [...p];
          nb[am] = 2;
          const rs = cw(nb);
          if (rs) {
            sw(rs.wn);
            swl(rs.ln);
          }
          return nb;
        });
      }
    });
    return () => sk.off('res');
  }, []);

  const hp = (i) => {
    if (b[i] !== 0 || w) return;
    const nb = [...b];
    nb[i] = 1;
    sb(nb);
    const rs = cw(nb);
    if (rs) {
      sw(rs.wn);
      swl(rs.ln);
    } else {
      sk.emit('req', nb.join(''));
    }
  };

  const rs = () => {
    sb(Array(9).fill(0));
    sw(null);
    swl([]);
  };

  const gm = (v, i) => {
    const iw = wl.includes(i);
    if (v === 1) return <span className={iw ? 'c_w' : 'c_b'}>X</span>;
    if (v === 2) return <span className={iw ? 'c_w' : 'c_b'}>O</span>;
    return '';
  };

  return (
    <div className="ct">
      <h1>TIC-TAC-TOE</h1>
      <div className="st">
        {w === 1 ? 'PLAYER 1 WON!' : w === 2 ? 'AI WON!' : w === 3 ? 'DRAW!' : '\u00A0'}
      </div>
      <div className="bd">
        {b.map((c, i) => (
          <div key={i} className="cl" onClick={() => hp(i)}>
            {gm(c, i)}
          </div>
        ))}
      </div>
      <button className="rb" onClick={rs}>RESET</button>
    </div>
  );
}

export default App;