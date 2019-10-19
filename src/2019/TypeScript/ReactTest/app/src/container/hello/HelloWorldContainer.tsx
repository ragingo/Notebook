import { connect } from 'react-redux';
import { State } from '../../reducers';
import { HelloWorldComponent } from './HelloWorldComponent';

const mapStateToProps = (state : State) => {
  return {
      name: state.testReducer.name
  }
}

const mapDispatchToProps = () => {
  return {
  }
}

export const HelloWorldContainer = connect(
  mapStateToProps,
  mapDispatchToProps
)(HelloWorldComponent);
