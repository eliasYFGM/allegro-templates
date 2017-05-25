#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

// Main state structure
struct State
{
   void ( *_load )( void *param );
   void ( *_free )( void );

   void ( *_enter )( void *param );
   void ( *_exit )( void );

   void ( *_pause )( void );
   void ( *_resume )( void );

   void ( *_update )( void );
   void ( *_draw )( BITMAP *bmp );

   // Private and used only by [engine.c]
   // Indicates if the state was already initialized.
   int loaded;
};

#endif // STATE_H_INCLUDED
