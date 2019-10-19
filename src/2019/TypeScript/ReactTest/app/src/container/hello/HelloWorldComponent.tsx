import React from 'react';

export const HelloWorldComponent = (props : {name: string}) => {
    return (
      <h2>Hello, {props.name} !</h2>
    );
}
