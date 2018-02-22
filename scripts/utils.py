def dict_product(params):
  return _dict_product(params)

def _dict_product(d, keys=None):
  if keys is None:
    keys = list(d.keys())

  if len(keys) == 1:
    k = keys[0]
    if not issubclass(type(d[k]), list):
      yield {k : d[k]}
    else:
      for v in d[k]:
        yield { k : v }
  else:
    k, keys = keys[-1], keys[:-1]

    for values in _dict_product(d, keys):
      if not issubclass(type(d[k]), list):
        result = values.copy()
        result[k] = d[k]
        yield result
      else:
        for v in d[k]:
          if issubclass(type(v), dict):
            for ev in dict_product(v):
              result = values.copy()
              result.update(ev)
              yield result
          else:
            result = values.copy()
            result[k] = v
            yield result
