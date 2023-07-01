#pragma once

enum MeasureType : int {
  Commit,
  Abort,
  Write,
  Read,
  Create,
  WTouch,
  RTouch,
  EPOTouch,
  DeleteAbort,
  DeleteEpo,
  DeleteOat,
  Urv1,
  Urv2,
  Size
};